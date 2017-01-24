#include <vorpal/core/Settings.hpp>

#include <vorpal/utility/asset/Content.hpp>
#include <vorpal/utility/asset/SimpleStorage.hpp>

void PrintHandlerContent(const vp::utility::asset::Handler& handler)
{
    printf("Handler \"%s\" status: %d\n",
        handler.GetName().c_str(),
        handler.IsValid());

    if (handler.IsValid())
    {
        const std::vector<uint8_t>& content = handler.GetContent().GetBuffer();
        printf("Handler \"%s\" content of size %zu\n",
            handler.GetName().c_str(),
            content.size());
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
    vp::core::Settings& settings = vp::core::Settings::Instance();

    settings.Init(argc, argv, "");
    settings.SetApplicationName("Asset simple storage test");

    vp::utility::asset::SimpleStorage& storage =
        vp::utility::asset::SimpleStorage::Instance();

    // Files should be located in WORKING_DIRECTORY of the test
    const std::string testFile1("derp.txt");
    const std::string testFile2("herp.txt");

    printf("Request sync %s\n", testFile1.c_str());
    vp::utility::asset::Handler derpHandler = storage.Get(testFile1);

    printf("Request sync %s\n", testFile1.c_str());
    vp::utility::asset::Handler derp2Handler = storage.Get(testFile1);

    printf("Request sync %s\n", testFile2.c_str());
    vp::utility::asset::Handler herpHandler = storage.Get(testFile2);

    printf("%s handlers are equal: %d\n",
        testFile1.c_str(),
        derpHandler == derp2Handler);
    if (derpHandler != derp2Handler)
        return EXIT_FAILURE;

    PrintHandlerContent(derpHandler);
    PrintHandlerContent(herpHandler);

    vp::utility::asset::SimpleStorage::Destroy();

    vp::core::Settings::Destroy();

    return EXIT_SUCCESS;
}
