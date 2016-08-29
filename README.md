# mm_import
Import lists from on server to another one.

To build it execute the following:
```
git clone git@github.com:jbs1/mm_import.git
qmake -project
qmake
make
```

The script is used like this
```
./mm_import <filename/path importlist> <owner-email> <list-password> <mailman-dir>
```
"import list" is a file containing list, that need to be imported, one per line.

"email" and "password" are the credentials used to create the lists.

"mailman-dir" is the dir where mailman is installed. (e.g. "/var/lib/mailman/")

for each list there have to be 3 files in the same dir as the "import list":
* <listname>: containing the config file, create with "bin/config_list -o"
* <listname>.subscribers: containing the subscribers, created with "bin/add_members -r"
* <listname>.mbox/<listname>.mbox: the archive dir copies form the private archive file
