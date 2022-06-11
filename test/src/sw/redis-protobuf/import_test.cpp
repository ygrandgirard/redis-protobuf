/**************************************************************************
   Copyright (c) 2022 sewenew

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
 *************************************************************************/

#include "import_test.h"
#include <unordered_map>
#include <string>
#include <chrono>
#include <thread>
#include "utils.h"

namespace sw {

namespace redis {

namespace pb {

namespace test {

void ImportTest::run() {
    auto key = test_key("import");

    KeyDeleter deleter(_redis, key);

    std::string name{"test_import.proto"};
    auto proto = R"(
syntax = "proto3";
package sw.redis.pb;
message Msg {
    int32 i = 1;
    string s = 2;
}
    )";
    REDIS_ASSERT(_redis.command<void>("PB.IMPORT", name, proto
            "failed to test pb.import command");

    // Ensure proto has been loaded
    std::this_thread::sleep_for(std::chrono::seconds(1));

    auto res = _redis.command<std::unordered_map<std::string, std::string>>("PB.LASTIMPORT");
    REDIS_ASSERT(res.size() == 1 && res[name] == "OK");

    REDIS_ASSERT(_redis.command<long long>("PB.SET", key, "sw.redis.pb.Msg", "/i", 123) &&
            _redis.command<long long>("PB.GET", key, "/i") == 123,
        "failed to test pb.import command");
}

}

}

}
