#include <gtest/gtest.h>
#include "Table.h"

TEST( TABLE_FUNCTIONALITY, CREATE_TABLE )
{
    Table table1( { "COL1", "COL2", "COL3", "COL4" } );
    auto table_columns_count = table1.columnsCount();
    EXPECT_EQ( table_columns_count, 4 );
}

TEST( TABLE_FUNCTIONALITY, INSERT )
{
    std::list<std::string> columns_names = { "COL1", "COL2", "COL3", "COL4" };
    Table table1( columns_names );
    table1.insert( { "1", "2", "3", "4" } );
    table1.insert( { "1", "2", "3" } );
    table1.insert( { "1", "2" } );
    table1.insert( { "1" } );
    table1.insert( {  } );
    for ( const auto& column_name : columns_names )
    {
        EXPECT_EQ( table1.columnValuesCount( column_name ), 4 );
    }
    
    
}

TEST( TABLE_FUNCTIONALITY, SELECT_ALL )
{
    std::list<std::string> columns_names = { "COL1", "COL2", "COL3", "COL4" };
    Table table1( columns_names );
    table1.insert( { "1", "2", "3", "4" } );
    table1.insert( { "1", "2", "3" } );
    table1.insert( { "1", "2" } );
    table1.insert( { "1" } );
    table1.insert( {  } );
    
    table1.selectAll( );
     
}

TEST( TABLE_FUNCTIONALITY, GROUP_BY )
{
    std::list<std::string> columns_names = { "A", "B", "C", "D" };
    Table table1( columns_names );
    table1.insert( { "1", "8", "ab", "qwerty" } );
    table1.insert( { "2", "2", "aa", "qwerty1" } );
    table1.insert( { "2", "2", "gh", "qwerty" } );
    table1.insert( { "4", "2", "aa", "qwerty2" } );
    table1.insert( { "5", "2", "le", "qwerty" } );
    
    table1.selectAll( );

    std::cout << std::endl;

    table1.groupBy( { "A", "B"}, { "C", "D" } );
     
}
