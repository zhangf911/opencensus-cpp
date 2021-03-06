// Copyright 2018, OpenCensus Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <cstdlib>
#include <string>
#include <thread>  // NOLINT

#include "absl/base/call_once.h"
#include "absl/strings/str_cat.h"
#include "benchmark/benchmark.h"
#include "include/grpc++/grpc++.h"
#include "opencensus/plugins/grpc/grpc_plugin.h"
#include "opencensus/plugins/grpc/internal/testing/echo.grpc.pb.h"
#include "opencensus/stats/stats.h"

namespace opencensus {
namespace {

absl::once_flag once;
void RegisterOnce() { absl::call_once(once, RegisterGrpcPlugin); }

class EchoServer final : public testing::EchoService::Service {
  ::grpc::Status Echo(::grpc::ServerContext* context,
                      const testing::EchoRequest* request,
                      testing::EchoResponse* response) override {
    if (request->status_code() == 0) {
      response->set_message(request->message());
      return ::grpc::Status::OK;
    } else {
      return ::grpc::Status(
          static_cast<::grpc::StatusCode>(request->status_code()), "");
    }
  }
};

// An EchoServerThread object creates an EchoServer on a separate thread and
// shuts down the server and thread when it goes out of scope.
class EchoServerThread final {
 public:
  EchoServerThread() {
    ::grpc::ServerBuilder builder;
    int port;
    builder.AddListeningPort("[::]:0", ::grpc::InsecureServerCredentials(),
                             &port);
    builder.RegisterService(&service_);
    server_ = builder.BuildAndStart();
    if (server_ == nullptr || port == 0) {
      std::abort();
    }
    server_address_ = absl::StrCat("[::]:", port);
    server_thread_ = std::thread(&EchoServerThread::RunServerLoop, this);
  }

  ~EchoServerThread() {
    server_->Shutdown();
    server_thread_.join();
  }

  const std::string& address() { return server_address_; }

 private:
  void RunServerLoop() { server_->Wait(); }

  std::string server_address_;
  EchoServer service_;
  std::unique_ptr<grpc::Server> server_;
  std::thread server_thread_;
};

void BM_E2eLatencyCensusDisabled(benchmark::State& state) {
  EchoServerThread server;
  std::unique_ptr<testing::EchoService::Stub> stub =
      testing::EchoService::NewStub(::grpc::CreateChannel(
          server.address(), ::grpc::InsecureChannelCredentials()));

  testing::EchoResponse response;
  for (auto _ : state) {
    testing::EchoRequest request;
    ::grpc::ClientContext context;
    ::grpc::Status status = stub->Echo(&context, request, &response);
  }
}
BENCHMARK(BM_E2eLatencyCensusDisabled);

void BM_E2eLatencyCensusEnabled(benchmark::State& state) {
  RegisterOnce();
  // This we can safely repeat, and doing so clears accumulated data to avoid
  // initialization costs varying between runs.
  RegisterGrpcViewsForExport();

  EchoServerThread server;
  std::unique_ptr<testing::EchoService::Stub> stub =
      testing::EchoService::NewStub(::grpc::CreateChannel(
          server.address(), ::grpc::InsecureChannelCredentials()));

  testing::EchoResponse response;
  for (auto _ : state) {
    testing::EchoRequest request;
    ::grpc::ClientContext context;
    ::grpc::Status status = stub->Echo(&context, request, &response);
  }
}
BENCHMARK(BM_E2eLatencyCensusEnabled);

}  // namespace
}  // namespace opencensus

BENCHMARK_MAIN();
