# data
data should be stored at both web server and judger when tests are running.
initially, a judger should get an empty data folder,
after calculating checksum or detect that there's just no such a folder,
the judger server should retrieve data files from web server.
if checksum of datafiles didn't change then just run it.


the data files should be named `[name].in` and `[name].out`
(optional if spj) in pair.

