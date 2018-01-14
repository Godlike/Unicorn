#include <unicorn/Settings.hpp>

#include <unicorn/utility/asset/Content.hpp>
#include <unicorn/utility/asset/SimpleStorage.hpp>

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
    unicorn::Settings& settings = unicorn::Settings::Instance();

    settings.Init(argc, argv, "SimpleStorageTest.log");
    settings.SetApplicationName("Asset simple storage test");

    unicorn::utility::asset::SimpleStorage& storage = unicorn::utility::asset::SimpleStorage::Instance();

    // Files should be located in WORKING_DIRECTORY of the test
    const std::string testFile1("derp.txt");
    const std::string testFile2("herp.txt");

    printf("Request sync %s\n", testFile1.c_str());
    unicorn::utility::asset::Handler derpHandler = storage.Get(testFile1);

    printf("Request sync %s\n", testFile1.c_str());
    unicorn::utility::asset::Handler derp2Handler = storage.Get(testFile1);

    printf("Request sync %s\n", testFile2.c_str());
    unicorn::utility::asset::Handler herpHandler = storage.Get(testFile2);

    printf("%s handlers are equal: %d\n", testFile1.c_str(), derpHandler == derp2Handler);
    if (derpHandler != derp2Handler)
    {
        return EXIT_FAILURE;
    }

    PrintHandlerContent(derpHandler);
    PrintHandlerContent(herpHandler);

    unicorn::utility::asset::SimpleStorage::Destroy();

    unicorn::Settings::Destroy();

    return EXIT_SUCCESS;
}
