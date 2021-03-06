#include "narwhal/narwhal.h"

TEST_FIXTURE(sample_collection, NarwhalCollection *)
{
    *sample_collection = narwhal_empty_collection();

    CLEANUP_FIXTURE(sample_collection)
    {
        while ((*sample_collection)->count > 0)
        {
            narwhal_collection_pop(*sample_collection);
        }

        narwhal_free_collection(*sample_collection);
    }
}

TEST(collection_initialization)
{
    NarwhalCollection *collection = narwhal_empty_collection();

    ASSERT_EQ(collection->count, (size_t)0);
    ASSERT_EQ(collection->first, NULL);
    ASSERT_EQ(collection->last, NULL);

    narwhal_free_collection(collection);
}

TEST(collection_operations, sample_collection)
{
    GET_FIXTURE(sample_collection);

    int numbers[] = { 0, 1, 2 };

    narwhal_collection_append(sample_collection, numbers);

    ASSERT_EQ(sample_collection->count, (size_t)1);
    ASSERT_EQ(sample_collection->first, sample_collection->last);

    narwhal_collection_append(sample_collection, numbers + 1);
    narwhal_collection_append(sample_collection, numbers + 2);

    ASSERT_EQ(sample_collection->count, (size_t)3);

    size_t i = 0;

    int *item;
    NARWHAL_EACH(item, sample_collection)
    {
        ASSERT_EQ(*item, numbers[i]);
        i++;
    }

    int *last = narwhal_collection_pop(sample_collection);

    ASSERT_EQ(*last, numbers[2]);
    ASSERT_EQ(sample_collection->count, (size_t)2);
}
