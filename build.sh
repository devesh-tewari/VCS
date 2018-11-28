g++ -o vcs VCS.cpp create_files_after_reset.cpp revert.cpp delete.cpp reset.cpp vcsdiff.cpp serialize.cpp objects.cpp init.cpp add.cpp status.cpp commit.cpp merge.cpp diff.cpp -lcrypto -std=c++11
cp vcs /usr/bin
#sudo apt install rcs #for merge
