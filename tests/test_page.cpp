#include <iostream>

#define BOOST_TEST_MODULE IndexTests
#include <boost/test/included/unit_test.hpp>

#include "page.h"

using namespace Spatium::Index;

BOOST_AUTO_TEST_CASE(TestPaging1)
{
    char page_ptr[4096]; 
    auto page = new (page_ptr) PageHeader(PageType::Leaf, 0, 4096);
    BOOST_CHECK_EQUAL(0, page->get_entries_count());
}

BOOST_AUTO_TEST_CASE(TestPaging2)
{
    char page_ptr[4096]; 
    auto page = new (page_ptr) PageHeader(PageType::Leaf, 0, 4096);
    auto free_space_before = page->get_free_space();
    char buffer[128];
    auto entry = new (buffer) Entry(128);
    auto result = page->add_entry(*entry);
    BOOST_CHECK_EQUAL(result, PageHeader::AddStatus::Success);
    auto free_space_after = page->get_free_space();
    BOOST_CHECK_EQUAL(free_space_before - free_space_after, 128 + sizeof(EntryOffset));
}

BOOST_AUTO_TEST_CASE(TestPaging3)
{
    char page_ptr[4096]; 
    auto page = new (page_ptr) PageHeader(PageType::Leaf, 0, 4096);
    char buffer[4096];
    auto entry = new (buffer) Entry(4096);
    auto result = page->add_entry(*entry);
    BOOST_CHECK_EQUAL(result, PageHeader::AddStatus::Overflow);
}

BOOST_AUTO_TEST_CASE(TestPaging4)
{
    char page_ptr[4096]; 
    auto page = new (page_ptr) PageHeader(PageType::Leaf, 0, 4096);
    char buffer[128];
    auto entry = new (buffer) Entry(1);
    auto result = page->add_entry(*entry);
    BOOST_CHECK_EQUAL(result, PageHeader::AddStatus::BadEntry);
}

BOOST_AUTO_TEST_CASE(TestPaging5)
{
    char page_ptr[4096]; 
    auto page = new (page_ptr) PageHeader(PageType::Leaf, 0, 4096);
    char buffer[222];
    auto entry = new (buffer) Entry(222);
    auto result = page->add_entry(*entry);
    BOOST_CHECK_EQUAL(result, PageHeader::AddStatus::Success);
    auto len = page->get_entry_length(0);
    BOOST_CHECK_EQUAL(len, 222);
}

BOOST_AUTO_TEST_CASE(TestPaging6)
{
    char page_ptr[4096]; 
    auto page = new (page_ptr) PageHeader(PageType::Leaf, 0, 4096);
    char buffer[64];
    TimeStamp inst = {1111L};
    auto entry = new (buffer) Entry(3333, inst, 64);
    for (int i = 0; i < 10; i++) {
        entry->value[i] = i + 1;
    }

    auto result = page->add_entry(*entry);
    BOOST_CHECK_EQUAL(result, PageHeader::AddStatus::Success);

    entry->param_id = 0;
    for (int i = 0; i < 10; i++) {
        entry->value[i] = i + 1;
    }
    TimeStamp inst2 = {1111L};
    entry->time = inst2;

    int len = page->copy_entry(0, entry);
    BOOST_CHECK_EQUAL(len, 64);
    BOOST_CHECK_EQUAL(entry->length, 64);
    BOOST_CHECK_EQUAL(entry->param_id, 3333);
}
