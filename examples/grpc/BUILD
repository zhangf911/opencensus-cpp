# Copyright 2018, OpenCensus Authors
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

load("//opencensus:copts.bzl", "DEFAULT_COPTS", "TEST_COPTS")
load("@com_github_grpc_grpc//bazel:cc_grpc_library.bzl", "cc_grpc_library")

licenses(["notice"])  # Apache License 2.0

package(default_visibility = ["//visibility:private"])

cc_grpc_library(
    name = "hello_proto",
    srcs = ["hello.proto"],
    proto_only = False,
    use_external = True,
    well_known_protos = False,
    deps = [],
)

cc_library(
    name = "stackdriver",
    srcs = ["stackdriver.cc"],
    hdrs = ["stackdriver.h"],
    copts = DEFAULT_COPTS,
    deps = [
        "//opencensus/exporters/stats/stackdriver:stackdriver_exporter",
        "//opencensus/exporters/trace/stackdriver:stackdriver_exporter",
        "@com_google_absl//absl/strings",
    ],
)

cc_binary(
    name = "hello_client",
    srcs = ["hello_client.cc"],
    copts = DEFAULT_COPTS,
    deps = [
        ":hello_proto",
        ":stackdriver",
        "//opencensus/exporters/stats/stackdriver:stackdriver_exporter",
        "//opencensus/exporters/stats/stdout:stdout_exporter",
        "//opencensus/exporters/trace/stackdriver:stackdriver_exporter",
        "//opencensus/exporters/trace/stdout:stdout_exporter",
        "//opencensus/plugins/grpc:grpc_plugin",
        "//opencensus/trace",
        "@com_github_grpc_grpc//:grpc++",
        "@com_google_absl//absl/strings",
    ],
)

cc_binary(
    name = "hello_server",
    srcs = ["hello_server.cc"],
    copts = DEFAULT_COPTS,
    deps = [
        ":hello_proto",
        ":stackdriver",
        "//opencensus/exporters/stats/prometheus:prometheus_exporter",
        "//opencensus/exporters/stats/stackdriver:stackdriver_exporter",
        "//opencensus/exporters/stats/stdout:stdout_exporter",
        "//opencensus/exporters/trace/stackdriver:stackdriver_exporter",
        "//opencensus/exporters/trace/stdout:stdout_exporter",
        "//opencensus/plugins/grpc:grpc_plugin",
        "//opencensus/trace",
        "@com_github_grpc_grpc//:grpc++",
        "@com_github_jupp0r_prometheus_cpp//:prometheus_cpp",
        "@com_google_absl//absl/strings",
    ],
)
