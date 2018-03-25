# Data
data should be stored at both web server and judger when tests are running.
initially, a judger should get an empty data folder,
after detecting that there's just no such a folder,
the judger server should retrieve data files from web server.
if version numbers of datafiles didn't change then just run it.


the data files should be named `[name].in` and `[name].out`
(optional if SPJ) in pair.

<!-- # checksum -->
<!-- Two methods are being considered. -->
<!-- 1. concatenate the bytes one by one and regard all files as one file. -->
<!-- 2. calculate the checksum of all files first and concatenate results. -->
<!-- then calculate checksum again. -->



# API in JSON
example:
```json
{
  "submission_id": 9527,
  "problem_id": 1001,
  "language": "cpp",
  "code": "#include <iostream>\n           #include <vector>\n           #include <cstring>\n           #include <algorithm>\n           using namespace std;\n           vector<int> tmp;\n           int dfs(int a){\n               if (a){\n                   tmp.push_back(a);\n                   return dfs(a - 1);\n               }\n\n               return 0;\n           }\n\n           int main(){\n               ios::sync_with_stdio(false);\n               cin.tie(nullptr);\n           //    char * t = new char [256 * 1024 * 1024];\n           //    memset(t, 0, sizeof(char) * 256 * 1024 * 1024);\n               int a = 1, b = 2;\n               while(cin >> a >> b){\n                   cout << a + b << endl;\n               }\n               return 0;\n\n           }",
  "time_limit": 1000,
  "memory_limit": 262144,
  "is_spj": false,
  "checksum": "32tqwet",
}
```

after the results emerged, the following infomation should be returned to the web server.
```json
{
  "submission_id": 9527,
  "final_result": "WA",
  "data_runs": {
     "first_data": {
     "result": "AC",
     "info": ""},
     "second_data": {
     "result": "AC",
      "info": ""},
     "third_data": {
     "result": "WA",
     "info": "you have different output on line 123,\n >>>> your output: \n 1 \n<<<< std output: 5\n" },
  }
  "judge_key": "shucam",
}
```
