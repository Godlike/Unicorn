#include <vorpal/core/Settings.hpp>

#include <vorpal/utility/asset/Content.hpp>
#include <vorpal/utility/asset/Storage.hpp>

void PrintHandlerContent(const vp::utility::asset::Handler& handler)
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

int main(int argc, char *argv[])
{
    vp::core::Settings& settings = vp::core::Settings::Instance();

    settings.Init(argc, argv, "");
    settings.SetApplicationName("Asset storage test");

    vp::utility::asset::Storage& storage = vp::utility::asset::Storage::Instance();

    storage.InitializeWorkers(10);
    storage.InitializeWorkers(1);

    // Files should be located in WORKING_DIRECTORY of the test
    const std::string testFile1( "derp.txt" );
    const std::string testFile2( "derpiness.txt" );
    const std::string testFile3( "herp.txt" );

    printf("Request async %s\n", testFile1.c_str());
    std::shared_future<vp::utility::asset::Handler> derp2Handler = storage.GetAsync(testFile1);

    printf("Request async %s with priority 200\n", testFile2.c_str());
    storage.GetAsync(testFile2, 200);

    printf("Request sync %s\n", testFile1.c_str());
    vp::utility::asset::Handler derpHandler = storage.Get(testFile1);

    printf("Request sync %s\n", testFile3.c_str());
    vp::utility::asset::Handler herpHandler = storage.Get(testFile3.c_str());

    printf("%s handlers are equal: %d\n", testFile1.c_str(), derpHandler == derp2Handler.get());
    if (derpHandler != derp2Handler.get()) return EXIT_FAILURE;

    PrintHandlerContent(derpHandler);
    PrintHandlerContent(herpHandler);

    vp::utility::asset::Storage::Destroy();

    vp::core::Settings::Destroy();

    return EXIT_SUCCESS;
}
