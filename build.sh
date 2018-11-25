<<<<<<< HEAD
g++ -o vcs VCS.cpp delete.cpp reset.cpp serialize.cpp objects.cpp init.cpp add.cpp status.cpp commit.cpp diff.cpp -lcrypto -std=c++11
=======
g++ -o vcs VCS.cpp revert.cpp vcsdiff.cpp serialize.cpp objects.cpp init.cpp add.cpp status.cpp commit.cpp merge.cpp diff.cpp -lcrypto -std=c++11
>>>>>>> 1792f787fa48125cef0d9bd11e07573663b43fbf
cp vcs /usr/bin
#sudo apt install rcs #for merge
