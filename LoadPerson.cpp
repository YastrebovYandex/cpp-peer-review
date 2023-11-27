struct SetDbConnectionParams{
	string db_name;
    int db_connection_timeout;
    bool db_allow_exceptions;
	
	SetDbConnectionParams& SetDbName(const string& db_name){
		this->db_name = db_name;
        return *this;
	}	
    SetDbConnectionParams& SetDbConnectionTimeout(int db_connection_timeout) {
        this->db_connection_timeout = db_connection_timeout;
        return *this;
    }
    SetDbConnectionParams& SetDbAllowExceptions(bool db_allow_exceptions) {
        this->db_allow_exceptions = db_allow_exceptions;
        return *this;
    }
};

struct SetPersonsParams{
    int min_age;
    int max_age;
        
    SetPersonsParams& SetMinAge(int min_age) {
        this->min_age = min_age;
        return *this;
    }
    SetPersonsParams& SetMaxAge(int max_age) {
        this->max_age = max_age;
        return *this;
    }
};

optional<DBHandler> DataBaseConnection(const SetDbConnectionParams& params, const DBLogLevel& db_log_level){
    DBConnector connector(params.db_allow_exceptions, db_log_level);
    DBHandler db;
    if (params.db_name.starts_with("tmp."s)) {
        db = connector.ConnectTmp(params.db_name, params.db_connection_timeout);
    } else {
        db = connector.Connect(params.db_name, params.db_connection_timeout);
    }
    if (!params.db_allow_exceptions && !db.IsOK()) {
        return nullopt;
    }
    return db;
}

vector<Person> LoadPersons(const SetDbConnectionParams& connection_db_params,
                           const DBLogLevel& db_log_level, const SetPersonsParams& person_params, string_view name_filter) {
    
    optional<DBHandler> db = DataBaseConnection( 
                       SetDbConnectionParams()
					   .SetDbName(connection_db_params.db_name)
                       .SetDbConnectionTimeout(connection_db_params.db_connection_timeout)
                       .SetDbAllowExceptions(connection_db_params.db_allow_exceptions),
                       db_log_level);

    if (db == nullopt) {
        return {};
    }
    
    ostringstream query_str;
    query_str << "from Persons "s
              << "select Name, Age "s
              << "where Age between "s << person_params.min_age << " and "s << person_params.max_age << " "s
              << "and Name like '%"s << db.Quote(name_filter) << "%'"s;
    DBQuery query(query_str.str());

    vector<Person> persons;
    for (auto [name, age] : db.LoadRows<string, int>(query)) {
        persons.push_back({move(name), age});
    }
    return persons;
}

