#include <iostream>
#include <mysql.h> // Include MySQL C API header

bool calculateSalesRevenueByDateRange(MYSQL* conn, const std::string& startDate, const std::string& endDate) {
    // Construct the SQL query to calculate sales revenue for the specified date range
    std::string query = "SELECT DATE_FORMAT(Rent_Date, '%Y-%m-%d') AS Rent_Date, TotalPrice FROM rent WHERE Rent_Date BETWEEN '" + startDate + "' AND '" + endDate + "'";

    // Add the sum of rental_price to the query
    query += " UNION ALL SELECT 'Total', SUM(TotalPrice) FROM rent WHERE Rent_Date BETWEEN '" + startDate + "' AND '" + endDate + "'";

    if (mysql_query(conn, query.c_str())) {
        std::cerr << "Error executing query: " << mysql_error(conn) << std::endl;
        return false;
    }

    MYSQL_RES* result = mysql_store_result(conn);
    if (!result) {
        std::cerr << "Error getting result: " << mysql_error(conn) << std::endl;
        return false;
    }

    int numFields = mysql_num_fields(result);
    MYSQL_ROW row;

    // Print table headers
    MYSQL_FIELD* fields = mysql_fetch_fields(result);
    for (int i = 0; i < numFields; ++i) {
        std::cout << fields[i].name << "\t";
    }
    std::cout << std::endl;

    // Print table data
    while ((row = mysql_fetch_row(result))) {
        for (int i = 0; i < numFields; ++i) {
            std::cout << row[i] << "\t";
        }
        std::cout << std::endl;
    }

    mysql_free_result(result);

    return true;
}

bool findMostRentedCarByDateRange(MYSQL* conn, const std::string& startDate, const std::string& endDate) {
    // Construct the SQL query to find the most rented car for the specified date range
    std::string query = "SELECT CarID, COUNT(*) AS rental_count FROM rent WHERE Rent_Date BETWEEN '" + startDate + "' AND '" + endDate + "' GROUP BY CarID ORDER BY rental_count DESC LIMIT 1";

    if (mysql_query(conn, query.c_str())) {
        std::cerr << "Error executing query: " << mysql_error(conn) << std::endl;
        return false;
    }

    MYSQL_RES* result = mysql_store_result(conn);
    if (!result) {
        std::cerr << "Error getting result: " << mysql_error(conn) << std::endl;
        return false;
    }

    int numFields = mysql_num_fields(result);
    MYSQL_ROW row;

    // Print table headers
    MYSQL_FIELD* fields = mysql_fetch_fields(result);
    for (int i = 0; i < numFields; ++i) {
        std::cout << fields[i].name << "\t";
    }
    std::cout << std::endl;

    // Print table data
    while ((row = mysql_fetch_row(result))) {
        for (int i = 0; i < numFields; ++i) {
            std::cout << row[i] << "\t";
        }
        std::cout << std::endl;
    }

    mysql_free_result(result);

    return true;
}