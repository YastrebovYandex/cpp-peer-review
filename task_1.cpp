#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

const int MAX_USER_COUNT = 100000;
const int MAX_PAGE_COUNT = 1000;

namespace cheer_reader  {
    void FillingVectorWithNumber(int number, vector<int>&  container){
        for(size_t i = 0; i != container.size(); ++i){
            container.at(i) = number;
        }
    }

    void UpdatePageRange(int lhs, int rhs, vector<int>&  number_readers_who_read_to_page) {
        for (int i = lhs; i < rhs; ++i) {
            ++number_readers_who_read_to_page[i];
        }
    }

    void Read(int user_id, int page_count, vector<int>&  page_reader_reached, vector<int>&  number_readers_who_read_to_page) {
        UpdatePageRange(page_reader_reached[user_id] + 1, page_count + 1, number_readers_who_read_to_page);
        page_reader_reached[user_id] = page_count;
    }

    double Cheer(int user_id, const vector<int>&  page_reader_reached,const  vector<int>&  number_readers_who_read_to_page) {
        const int pages_count = page_reader_reached[user_id];
        if (pages_count == -1) {
            return 0;
        }
        const int user_count = number_readers_who_read_to_page[0];
        if (user_count == 1) {
            return 1;
        }

        return static_cast<double>(user_count - number_readers_who_read_to_page[pages_count]) / (user_count - 1);
    }
}

int main()
{
    vector<int> page_reader_reached;
    vector<int> number_readers_who_read_to_page;

    page_reader_reached.resize(MAX_USER_COUNT + 1);
    number_readers_who_read_to_page.resize(MAX_PAGE_COUNT + 1);
    cheer_reader::FillingVectorWithNumber(-1, page_reader_reached);

    int query_count;
    cin >> query_count;

    for (int query_id = 0; query_id < query_count; query_id++) {
        string query_type;
        cin >> query_type;
        int user_id;
        cin >> user_id;

        if (query_type == "READ"s) {
            int page_count;
            cin >> page_count;
            cheer_reader::Read(user_id, page_count, page_reader_reached, number_readers_who_read_to_page);
        } else if (query_type == "CHEER"s) {
            cout << setprecision(6) << cheer_reader::Cheer(user_id, page_reader_reached, number_readers_who_read_to_page) << endl;
        }
    }
    return 0;
}
