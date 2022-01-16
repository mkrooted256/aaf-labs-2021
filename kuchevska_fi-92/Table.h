#include <unordered_map>
#include <list>
#include <string>
#include <iostream>
#include <algorithm>
#include <map>
/*
Вам потрібно зробити клас Table, яким можна буде користуватись якось так:
- Створити екземпляр класу, передавши в конструктор список імен стовпців.
- Додати до таблиці рядок, викликавши метод insert.
- Вивести на екран всю таблицю методом selectAll.
- Виконати агрегацію і вивести на екран результат, методом groupBy.
*/
using group = std::list<std::string>;
using min_max = std::list<std::pair<std::string, std::string>>;
class Table
{
public:
    Table( const std::list<std::string>& columns_names );
    void insert( const std::list< std::string >& values );
    void selectAll( ) const;
    std::map<group, min_max> groupBy( const std::list<std::string>& columns_group_by,
                    const std::list<std::string>& columns_functions_by ) const;
    size_t columnsCount( ) const;
    size_t columnValuesCount( const std::string& column_name ) const;
private:
    std::unordered_map< std::string, std::list<std::string> > table;
    std::list<std::string> columns_names;

};

Table::Table( const std::list<std::string>& columns_names )
{
    for ( auto rit = std::rbegin( columns_names ); rit != std::rend( columns_names ); ++rit )
    {
        table[ *rit ] = {};
    }

    this->columns_names = columns_names;
    
}

void 
Table::insert( const std::list< std::string >& values )
{
    if ( !values.empty( ) )
    {
        auto it = std::begin( columns_names );
        for ( auto value : values )
        {
            table[*it].push_back( std::move( value ) );
            ++it;
        }

        for ( ; it != std::end( columns_names ); ++it )
        {
            table[*it].push_back( "" );
        }
    }
       
}

void 
Table::selectAll( ) const
{
    //print COLUMN NAMES
    //pair = ( COLUMN NAME, LIST OF ALL VALUES IN COLUMN )
    for ( auto str : columns_names )
    {
        std::cout << "|" << std::setw( 10 ) << str << " ";
    }
    std::cout << std::endl;

    auto lines_counts = (*std::begin( table )).second.size( );

    for ( size_t i = 0; i < lines_counts; ++i )
    {
        //pair = ( COLUMN NAME, LIST OF ALL VALUES IN COLUMN )
        for ( auto str : columns_names )
        {
            auto& values_list = table.at( str );
            auto our_value = std::next( std::begin( values_list ), i );
            std::cout << "|" << std::setw( 10 ) << *our_value << " ";
        }
        std::cout << std::endl;
    }
    
    
}

size_t 
Table::columnsCount( ) const
{
    return table.size( );
}

size_t 
Table::columnValuesCount( const std::string& column_name ) const
{
    try
    {
        return table.at( column_name ).size( );
    }
    catch( std::out_of_range )
    {
        return 0;
    }
        
}

void output_groupby( const std::map<group, min_max>& group_by );

std::map<group, min_max>
Table::groupBy( const std::list<std::string>& columns_group_by,
                const std::list<std::string>& columns_functions_by ) const
{
    
    using lists_in_group = std::list<std::list<std::string>>;

    std::map<group, min_max> group_by;
    if ( !columns_group_by.empty( ) )
    {
        std::map<group, lists_in_group> temp_table;

        auto lines_counts = (*std::begin( table )).second.size( );

        std::list<std::list<std::string>> values_in_columns;
        for ( const auto& column_name : columns_group_by )
        {
            values_in_columns.push_back( table.at( column_name ) );
        }

        std::list<std::list<std::string>> values_in_func_columns;
        for ( const auto& column_name : columns_functions_by )
        {
            values_in_func_columns.push_back( table.at( column_name ) );
        }
        
        std::list<group> our_groups;
        std::list<group> our_funcs_values;

        for (size_t j = 0; j < lines_counts; j++)
        {
            std::list<std::string> temp;
            for (size_t i = 0; i < values_in_columns.size(); i++)
            {
                auto value = *std::next( std::begin( *std::next( std::begin( values_in_columns ), i ) ), j );
                temp.push_back( value );
            }

            our_groups.push_back( temp );
            temp.clear( );
            for (size_t i = 0; i < values_in_func_columns.size(); i++)
            {
                auto value = *std::next( std::begin( *std::next( std::begin( values_in_func_columns ), i ) ), j );
                temp.push_back( value );
            }
            our_funcs_values.push_back( temp );
        }

        auto func_it = our_funcs_values.begin( );
        for ( auto our_group : our_groups )
        {
            temp_table[ our_group ].push_back( *func_it );
            ++func_it;
        }
        
        for ( const auto& pair : temp_table )
        {
            auto lines_counts = (*std::begin( pair.second )).size( );
           
            for (size_t j = 0; j < lines_counts; j++)
            {
                std::list<std::string> temp;
                for (size_t i = 0; i < pair.second.size(); i++)
                {
                    auto value = *std::next( std::begin( *std::next( std::begin( pair.second ), i ) ), j );
                    temp.push_back( value );
                }

                auto min_max_el = std::minmax_element( temp.begin( ), temp.end( ) );
                std::pair<std::string, std::string> min_max_pair( { *min_max_el.first, *min_max_el.second } );
                group_by[ pair.first ].push_back( min_max_pair );
            }
        }
        
    }

    output_groupby( group_by );
    
    return group_by;
}

void output_groupby( const std::map<group, min_max>& group_by )
{
    for ( const auto& pair : group_by )
    {
        for ( const auto& el : pair.first )
        {
            std::cout << el << " ";
        }

        for ( const auto& el : pair.second )
        {
            std::cout << el.first << " " << el.second << " ";
        }
        std::cout << std::endl;
        
        
    }
    
}