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

    printf("Request async /tmp/derp\n");
    std::shared_future<vp::utility::asset::Handler> derp2Handler = storage.GetAsync("/tmp/derp");

    printf("Request async /tmp/derpiness with priority 200\n");
    storage.GetAsync("/tmp/derpiness", 200);

    printf("Request sync /tmp/derp\n");
    vp::utility::asset::Handler derpHandler = storage.Get("/tmp/derp");

    printf("Request sync /tmp/herp\n");
    vp::utility::asset::Handler herpHandler = storage.Get("/tmp/herp");

    printf("/tmp/derp handlers are equal: %d\n", derpHandler == derp2Handler.get());
    if (derpHandler != derp2Handler.get()) return EXIT_FAILURE;

    PrintHandlerContent(derpHandler);
    PrintHandlerContent(herpHandler);

    vp::utility::asset::Storage::Destroy();

    vp::core::Settings::Destroy();

    return EXIT_SUCCESS;
}
