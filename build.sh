g++ -o vcs VCS.cpp delete.cpp reset.cpp serialize.cpp objects.cpp init.cpp add.cpp status.cpp commit.cpp diff.cpp -lcrypto -std=c++11
cp vcs /usr/bin
#sudo apt install rcs #for merge
