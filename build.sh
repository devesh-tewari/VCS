g++ -o vcs VCS.cpp serialize.cpp objects.cpp init.cpp add.cpp status.cpp -lcrypto
cp vcs /usr/bin
#sudo apt install rcs #for merge
