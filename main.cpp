#include <bits/stdc++.h>
using namespace std;
// generic functions 
string toLowerCase(const string& str) {
    string result;
    // result.reserve(str.length());

    for (char c : str) {
        if(c>='A' && c<='Z'){
            result += tolower(c);
        }
        else if(c!=' '){
            result += c;
        }
    }

    return result;
}

string removeBOM(const string& str) {
    if (str.size() >= 3 && static_cast<unsigned char>(str[0]) == 0xEF &&
        static_cast<unsigned char>(str[1]) == 0xBB &&
        static_cast<unsigned char>(str[2]) == 0xBF) {
        return str.substr(3);
    }
    return str;
}

void printCSVSheet(const vector<vector<string>>& sheet) {
    // Iterate over rows
    for (const auto& row : sheet) {
        // Iterate over cells in the row
        for (const auto& cell : row) {
            cout << cell << "\t";  // Print cell value
        }
        cout << endl;  // Move to the next row
    }
    cout << endl;  // Add a blank line between sheets
}


// data structures used
map<string,vector<string>> station_lines;// station - line 
map<string,int>station_nodes; // station - integer node
map<int,string>r_station_nodes; // integer node - station
map<string,string>original_name; // station - Printable name
map<string,set<int>> intersection; // line - all the intersecting stations 
vector<vector<vector<int>>> distance_matrix; // here first string in pair represent which line and second int represent the node
map<string,int> line_map; // line - integer val;


// to make graph
void graph_maker(vector<vector<vector<string>>> &lines){
    int count = 0;
    int x = 0;
    for(auto line:lines){
        for(auto station:line){
            string station_name = toLowerCase(station[0]);
            string line_name = station[1];
            station_lines[station_name].push_back(line_name);
            if(!station_nodes.count(station_name)){
                station_nodes[station_name]=x;
                r_station_nodes[x]=station_name;
                x++;
            }
            original_name[station_name]=station[0];
            line_map[line_name]=count;
        }
        count++;
    }
    distance_matrix.resize(300,vector<vector<int>>(300,vector<int>(10,INT_MAX)));
    count=0;
    for(auto line:lines){
        for(int i=0;i<line.size();i++){
            int node = station_nodes[toLowerCase(line[i][0])];
            if(station_lines[toLowerCase(line[i][0])].size()>1){
                intersection[line[i][1]].insert(node);
            }
            for(int j=0;j<line.size();j++){
                int node1 = station_nodes[toLowerCase(line[j][0])];
                distance_matrix[node][node1][line_map[line[i][1]]]=abs(i-j)+1;
            }
        }
        count++;
    }
}
// csv file reader function

void readCSVFile(const string& filePath) {
    ifstream file(filePath);
    if(file.is_open()){
        string line;
        vector<vector<string>> sheet;
        vector<vector<vector<string>>> lines;
        while(getline(file, line)){
            if(line.size()==1){
                // Empty line indicates the end of a sheet
                // cout<<sheet.size()<<endl;
                lines.push_back(sheet);
                sheet.clear();
            }
            else{
                // Split the line by commas to get the cells
                vector<string> cells;
                stringstream ss(line);
                string cell;
                while (getline(ss, cell, ',')) {
                    
                    cells.push_back(removeBOM(cell));
                }
                sheet.push_back(cells);
            }
        }
        if(!sheet.empty()){
            lines.push_back(sheet);
        }
        graph_maker(lines);
        file.close();
    }
    else{
        cout << "Failed to open the file: " << filePath << endl;
    }
}

// dijkstra's Algorithm

// 2 find and print the shortest path

void Print_shortest_path(string source_node,string destination_node){
    int s_node = station_nodes[source_node];
    int d_node = station_nodes[destination_node];
    vector<string> source_line = station_lines[source_node];
    vector<string> destination_line = station_lines[destination_node];
    set<string> final_line = {destination_line.begin(),destination_line.end()};
    priority_queue<vector<int>,vector<vector<int>>,greater<vector<int>>> q;                                                      // distance - node
    vector<int> distance(300,INT_MAX);
    vector<int> parent(300,-1);
    distance[s_node]=0;
    q.push({0,s_node});
    while(q.size()){
        vector<int> temp = q.top();
        q.pop();
        string station_name = r_station_nodes[temp[1]];
        int dist = temp[0];
        for(auto i:station_lines[station_name]){
            if(final_line.find(i)!=final_line.end()){
                if(distance[d_node]>(temp[0]+distance_matrix[temp[1]][d_node][line_map[i]])){
                    parent[d_node]=temp[1];
                    distance[d_node]=temp[0]+distance_matrix[temp[1]][d_node][line_map[i]];
                }
                continue;
            }
            for(auto j:intersection[i]){
                if(distance[j]>dist+distance_matrix[j][temp[1]][line_map[i]]){
                    distance[j]=dist+distance_matrix[j][temp[1]][line_map[i]];
                    parent[j]=temp[1];
                    q.push({distance[j],j});
                }
            }
        }
    }
    int it = d_node;
    vector<string> interchange;
    while(it!=-1){
        interchange.push_back(original_name[r_station_nodes[it]]);
        it=parent[it];
    }
    int number_of_stations = distance[d_node];
    int number_of_interchange = interchange.size()-2;
    reverse(interchange.begin(),interchange.end());
    int total_number_of_station = number_of_stations-number_of_interchange;
    cout<<endl;
    cout<<"Number of stations are = "<<total_number_of_station<<endl;
    cout<<endl;
    cout<<"Total Time taken = "<<(number_of_interchange*10)+(total_number_of_station)*2<<"mins"<<endl;
    cout<<endl;
    if(total_number_of_station<=2){
        cout<<"Cost on Token = "<<"10rs"<<endl;
        cout<<"Cost on Card = "<<"8-9rs"<<endl;
    }
    else if(total_number_of_station<=5){
        cout<<"Cost on Token = "<<"20rs"<<endl;
        cout<<"Cost on Card = "<<"16-18rs"<<endl;
    }
    else if(total_number_of_station<=12){
        cout<<"Cost on Token = "<<"30rs"<<endl;
        cout<<"Cost on Card = "<<"24-27rs"<<endl;
    }
    else if(total_number_of_station<=21){
        cout<<"Cost on Token = "<<"40rs"<<endl;
        cout<<"Cost on Card = "<<"32-36rs"<<endl;
    }
    else if(total_number_of_station<=32){
        cout<<"Cost on Token = "<<"50rs"<<endl;
        cout<<"Cost on Card = "<<"40-45rs"<<endl;
    }
    else{
        cout<<"Cost on Token = "<<"60rs"<<endl;
        cout<<"Cost on Card = "<<"48-54rs"<<endl;
    }
    cout<<endl;
    for(int i=0;i<interchange.size()-1;i++){
        cout<<interchange[i]<<" -> ";
    }
    cout<<*interchange.rbegin()<<endl;
    cout<<endl;
}

int main() {
    string filePath = R"(C:\Users\aksha\OneDrive\Desktop\Delhi Metro App\Delhi_Metro_Data.csv)"; // enter the csv file path
    readCSVFile(filePath);
    cout << "Welcome to Delhi Metro App" << endl;
    cout << "Choose an option:" << endl;
    cout << "1. Find a station" << endl;
    cout << "2. Find the shortest path between two stations" << endl;
    int option;
    cout<<endl;
    cout << "Enter your option: ";
    cin >> option;

    if (option == 1) {
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore remaining characters on the current line
        string station;
        cout<<endl;
        cout << "Enter the station name: ";
        cout<<endl;
        getline(std::cin, station);
        station = toLowerCase(station);

        // Check if the station exists
        if (original_name.count(station)) {
            cout << "Station found: " << original_name[station] << endl;
            cout << "Lines connected to this station: ";
            for (auto i : station_lines[station]) {
                cout << i << " ";
            }
            cout << endl;
        } else {
            cout << "Station not found." << endl;
        }
    }
    else if(option==2){
        int time_for_input = 3;
        cout<<endl;
        cout<<"Enter the First Station = ";
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore remaining characters on the current line
        string station1;
        getline(std::cin, station1);
        station1 = toLowerCase(station1);
        cout<<"Enter the Second Station = ";
        string station2;
        getline(std::cin, station2);
        station2 = toLowerCase(station2);
        while((!station_nodes.count(station1) || !station_nodes.count(station2)) && time_for_input){
            if(station_nodes.count(station1)){
                cout << "Second Station not found!"<<endl;
                cout<<endl;
                cout<<"Enter the Second Station = ";
                getline(std::cin, station2);
                station2 = toLowerCase(station2);
                cout<<endl;
            }
            else if(station_nodes.count(station2)){
                cout << "First Station not found!"<<endl;
                cout<<endl;
                cout<<"Enter the First Station = ";
                getline(std::cin, station1);
                cout<<endl;
                station1 = toLowerCase(station1);
            }
            else{
                cout << "First Station not found!"<<endl;
                cout<<endl;
                cout<<"Enter the First Station = ";
                getline(std::cin, station1);
                cout<<endl;
                station1 = toLowerCase(station1);
                cout << "Second Station not found!"<<endl;
                cout<<endl;
                cout<<"Enter the First Station = ";
                getline(std::cin, station2);
                station2 = toLowerCase(station2);
                cout<<endl;
            }
            time_for_input--;
        }
        if(time_for_input==0){
            cout<<"Sorry but the input limit has been fulfilled!"<<endl;
            cout<<endl;
            cout<<"Try again after some time"<<endl;
            cout<<endl;
        }
        else{
            Print_shortest_path(station1,station2);
        }
    }
    else{
        cout<<"Please pick the right option and Try Again!"<<endl;
    }
    cout << "Thank you for using Delhi Metro App!" << endl;
    return 0;
}
