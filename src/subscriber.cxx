#include <iceoryx_hoofs/cxx/optional.hpp>
#include <iceoryx_hoofs/posix_wrapper/signal_handler.hpp>
#include <iceoryx_posh/popo/subscriber.hpp>
#include <iceoryx_posh/popo/user_trigger.hpp>
#include <iceoryx_posh/popo/wait_set.hpp>
#include <iceoryx_posh/runtime/posh_runtime.hpp>
#include <include/topic_data.hxx>

#include <atomic>
#include <iostream>

//! [sig handler]
std::atomic_bool keep_running{true};
iox::cxx::optional<iox::popo::WaitSet<>> wait_set;

static void sigHandler(int sig IOX_MAYBE_UNUSED)
{
    keep_running = false;
    if (wait_set)
    {
        wait_set->markForDestruction();
    }
}
//! [sig handler]

int main()
{
    // initialize runtime
    iox::runtime::PoshRuntime::initRuntime("subscriber");

    // create waitset inside of the optional
    //! [create waitset]
    wait_set.emplace();

    // register signal handler to handle termination of the loop
    auto signal_guard =
        iox::posix::registerSignalHandler(iox::posix::Signal::INT, sigHandler).expect("failed to register SIGINT");
    auto signal_term_guard =
        iox::posix::registerSignalHandler(iox::posix::Signal::TERM, sigHandler).expect("failed to register SIGTERM");

    // create subscriber
    iox::popo::Subscriber<MessageTopic> subscriber({"Quaternion", "FrontLeft", "Counter"});

    // attach subscriber to waitset
    wait_set->attachState(subscriber, iox::popo::SubscriberState::HAS_DATA).or_else([](auto)
                                                                                    {
        std::cerr << "failed to attach subscriber" << std::endl;
        std::exit(EXIT_FAILURE); });
    //! [create waitset]

    //! [mainloop]
    while (keep_running)
    {
        // We block and wait for samples to arrive.
        auto notificationVector = wait_set->wait();

        for (auto &notification : notificationVector)
        {
            // We woke up and hence there must be at least one sample. When the sigHandler has called
            // markForDestruction the notificationVector is empty otherwise we know which subscriber received samples
            // since we only attached one.
            // Best practice is to always acquire the notificationVector and iterate over all elements and then react
            // accordingly. When this is not done and more elements are attached to the WaitSet it can cause
            // problems since we either miss events or handle events for objects which never occurred.
            if (notification->doesOriginateFrom(&subscriber))
            {
                // Consume a sample
                subscriber.take()
                    .and_then([](auto &sample)
                              { auto& data = sample->data;
                                std::cout << "got value: " << data.w << ", " << data.x << ", " << data.y << ", " << data.z << std::endl;
                                std::cout << " counter: " << sample->counter << std::endl; })
                    .or_else([](auto &reason)
                             { std::cout << "got no data, return code: " << static_cast<uint64_t>(reason) << std::endl; });
                // We could consume all samples but do not need to.
                // If there is more than one sample we will wake up again since the state of the subscriber is still
                // iox::popo::SubscriberState::HAS_DATA in this case.
            }
        }
    }
    //! [mainloop]

    std::cout << "shutting down" << std::endl;

    wait_set.reset();
    return (EXIT_SUCCESS);
}