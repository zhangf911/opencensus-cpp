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

#ifndef OPENCENSUS_EXPORTERS_STATS_STDOUT_STDOUT_EXPORTER_H_
#define OPENCENSUS_EXPORTERS_STATS_STDOUT_STDOUT_EXPORTER_H_

namespace opencensus {
namespace exporters {
namespace stats {

// A stats exporter that exports views registered with StatsExporter to stdout.
// StdoutExporter is immutable.
class StdoutExporter {
 public:
  static void Register();

 private:
  class Handler;
};

}  // namespace stats
}  // namespace exporters
}  // namespace opencensus

#endif  // OPENCENSUS_EXPORTERS_STATS_STDOUT_STDOUT_EXPORTER_H_
