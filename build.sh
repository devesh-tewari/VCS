g++ -o vcs VCS.cpp revert.cpp vcsdiff.cpp serialize.cpp objects.cpp init.cpp add.cpp status.cpp commit.cpp merge.cpp diff.cpp -lcrypto -std=c++11
cp vcs /usr/bin
#sudo apt install rcs #for merge
