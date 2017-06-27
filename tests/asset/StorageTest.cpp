#include <unicorn/core/Settings.hpp>

#include <unicorn/utility/asset/Content.hpp>
#include <unicorn/utility/asset/Storage.hpp>

void PrintHandlerContent(const unicorn::utility::asset::Handler& handler)
{
    printf("Handler \"%s\" status: %d\n", handler.GetName().c_str(), handler.IsValid());

    if (handler.IsValid())
    {
        const std::vector<uint8_t>& content = handler.GetContent().GetBuffer();
        printf("Handler \"%s\" content of size %zu\n", handler.GetName().c_str(), content.size());

        if (!content.empty())
        {
            for (auto i : content)
            {
                printf("%x ", i);
            }

            printf("\n");
        }
    }
}

int main(int argc, char* argv[])
{
    unicorn::core::Settings& settings = unicorn::core::Settings::Instance();

    settings.Init(argc, argv, "");
    settings.SetApplicationName("Asset storage test");

    unicorn::utility::asset::Storage& storage = unicorn::utility::asset::Storage::Instance();

    storage.InitializeWorkers(10);
    storage.InitializeWorkers(1);

    // Files should be located in WORKING_DIRECTORY of the test
    const std::string testFile1("derp.txt");
    const std::string testFile2("derpiness.txt");
    const std::string testFile3("herp.txt");

    printf("Request async %s\n", testFile1.c_str());
    std::shared_future<unicorn::utility::asset::Handler> derp2Handler = storage.GetAsync(testFile1);

    printf("Request async %s with priority 200\n", testFile2.c_str());
    storage.GetAsync(testFile2, 200);

    printf("Request sync %s\n", testFile1.c_str());
    unicorn::utility::asset::Handler derpHandler = storage.Get(testFile1);

    printf("Request sync %s\n", testFile3.c_str());
    unicorn::utility::asset::Handler herpHandler = storage.Get(testFile3.c_str());

    printf("%s handlers are equal: %d\n", testFile1.c_str(), derpHandler == derp2Handler.get());
    if (derpHandler != derp2Handler.get())
    {
        return EXIT_FAILURE;
    }

    PrintHandlerContent(derpHandler);
    PrintHandlerContent(herpHandler);

    unicorn::utility::asset::Storage::Destroy();

    unicorn::core::Settings::Destroy();

    return EXIT_SUCCESS;
}
