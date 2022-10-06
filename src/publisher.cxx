#include <iceoryx_dust/posix_wrapper/signal_watcher.hpp>
#include <iceoryx_posh/popo/publisher.hpp>
#include <iceoryx_posh/runtime/posh_runtime.hpp>
#include <include/topic_data.hxx>

#include <chrono>
#include <iostream>

constexpr uint32_t period_time_ms = 100;
void sending()
{
    iox::runtime::PoshRuntime::initRuntime("publisher-example");
    iox::popo::Publisher<MessageTopic> publisher({"Quaternion", "FrontLeft", "Counter"});

    for (uint32_t counter = 0U; !iox::posix::hasTerminationRequested(); ++counter)
    {
        publisher.publishCopyOf(MessageTopic(counter))
            .and_then([&]
                      { std::cout << "Sending: " << counter << std::endl; })
            .or_else([&](auto)
                     { std::cout << "Failed sending: " << counter << std::endl; });

        std::this_thread::sleep_for(std::chrono::milliseconds(period_time_ms));
    }
}

int main()
{
    std::thread tx(sending);
    tx.join();

    return (EXIT_SUCCESS);
}