vcs init
vcs add test
vcs commit -m "initial"
cat >> test/abcd
vcs -b checkout new
vcs reset
vcs add test
vcs commit -m "inNew"
vcs checkout master
cat >> test/abcd
vcs reset
vcs add test
vcs commit -m "inMaster"
vcs merge new
