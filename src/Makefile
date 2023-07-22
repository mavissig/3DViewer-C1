#Copyright (c) 2023
#Project: 3D_Viewer
#Version: 1.0
#Developer: Egor Kondratov
#Сompiler configuration
CC=gcc
CFLAGS=-Wall -Werror -Wextra -g -std=c11 -lm $(COPTIMIZATION_FLAGS)
COPTIMIZATION_FLAGS=-O2
CCOVERAGE_FLAGS=-fprofile-arcs -ftest-coverage

#OS
COS=$(shell uname)

#Directories
CBUILD_DIR=build/
CGUIDE_DIR=guide/
CHTML_DIR=$(CGUIDE_DIR)html/
CREPORT_DIR=report/
CTEST_DIR=test/
CLOGS_DIR=logs/
CPROJECT_DIR=Viewer_v1_0/
CREPORT_DIR=report/
CSOURCE=sources/
CHEADERS=headers/
CFRONTEND=$(CPROJECT_DIR)frontend/
CBACKEND=$(CPROJECT_DIR)backend/
CSOURCE_BACKEND=$(CBACKEND)$(CSOURCE)
CHEADERS_BACKEND=$(CBACKEND)$(CHEADERS)
CSOURCE_FRONTEND=$(CFRONTEND)$(CSOURCE)
CHEADERS_FRONTEND=$(CFRONTEND)$(CHEADERS)

#Project сonfiguration
CARCHIVE_DIR=archive/
CARCHIVE_NAME=Viewer_v1_0.zip
CMANUAL=$(CHTML_DIR)index.html
CAPP_NAME=Viewer_v1_0
CPRO_CONFIG=$(CPROJECT_DIR)$(CAPP_NAME).pro
ifeq ($(shell qmake --version | grep -o QMake),QMake)
	QMAKE=qmake
else
	$(error You must install qmake, before install project)
endif
CINSTALL=$(QMAKE) $(CPRO_CONFIG) -o $(CBUILD_DIR)Makefile && make -C $(CBUILD_DIR)
ifeq ($(COS),Linux)
	CAPP_PATH=$(CBUILD_DIR)
	COPTIMIZATION_FLAGS+=-flto
else
	CAPP_PATH=$(CBUILD_DIR)Viewer_v1_0.app/Contents/MacOS/
endif

#Gcov configuration
CREPORT=$(CREPORT_DIR)report/index.html
ifeq ($(COS), Linux)
	COPEN=xdg-open
else
	COPEN=open -a "Google Chrome"
endif

#Test configuration
CTEST=$(CTEST_DIR)test.c
CRUNNER=$(CBUILD_DIR)runner
CLEAK_LOG=$(CLOGS_DIR)test_log.txt
ifeq ($(shell pkg-config --help | grep -o "\--help"),--help)
ifeq ($(shell pkg-config --cflags --libs check | grep -o lcheck),lcheck)
	CTEST_FLAGS=$(shell pkg-config --cflags --libs check)
else
	CTEST_FLAGS=-lcheck -lsubunit -lrt
endif
else
ifeq ($(COS),Darwin)
	CTEST_FLAGS=-lcheck
else
	CTEST_FLAGS=-lcheck -lsubunit -lrt
endif
endif
CTEST_FLAGS+=-pthread

#Leak configuration
CLOG_NAME=$(CLOGS_DIR)leak_log.txt
ifeq ($(COS),Darwin)
	CLEAK=leaks --atExit --
else
	CLEAK=valgrind --leak-check=full -s --track-origins=yes
endif

#Git configuration
com="auto save"
CBRANCH=$(shell git branch | grep '*' | grep -o " [a-z]*-[a-z,1-9]*")

#Base commands
all: tests install

clean: uninstall
	rm -rf $(CTEST_DIR)*.dSYM/ .DS_Store *.o $(CREPORT_DIR)*.gcda $(CREPORT_DIR)*.gcno *.a $(CRUNNER)
	rm -rf $(CREPORT_DIR)report $(CREPORT_DIR)*.info $(CREPORT_DIR)*.index $(CREPORT_DIR)gcov_report
	rm -rf $(CBUILD_DIR)*.dSYM *.out vgcore* *.data *.heap $(CLIB_DIR)*.a $(CBUILD_DIR)*.dSYM
	rm -rf *.zip* *.tar* $(CBUILD_DIR)ui_*.h $(CBUILD_DIR)moc_*.h $(CREPORT_DIR)*.dSYM $(CBUILD_DIR)*.tmp
	rm -rf $(CBUILD_DIR)Makefile $(CBUILD_DIR)*.cpp $(CBUILD_DIR).qmake.stash $(CBUILD_DIR)*.o *.gcno *.gcda
	rm -rf build-Viewer_v1_0-* $(CLOGS_DIR)*.old $(CBUILD_DIR)*.so $(CGUIDE_DIR)Doxyfile.bak
	rm -rf $(CARCHIVE_DIR)/*.*
	rm -f *.bmp *.jpg *.jpeg

tests: objective
	$(CC) $(CFLAGS) *.o $(CTEST) -o $(CRUNNER) $(CTEST_FLAGS)
	rm -rf *.o

objective:
	$(CC) $(CFLAGS) -c $(CSOURCE_BACKEND)*.c

run_test:
	./$(CRUNNER)

gcov_report:
ifeq ($(shell lcov --version | grep -o LCOV),LCOV)
	$(CC) $(CFLAGS) $(CSOURCE_BACKEND)*.c $(CTEST) -o $(CREPORT_DIR)gcov_report $(CCOVERAGE_FLAGS) $(CTEST_FLAGS) 
	./$(CREPORT_DIR)gcov_report
	lcov -t "Viewer test" -o $(CREPORT_DIR)test.info -c -d .
	genhtml -o $(CREPORT_DIR)report $(CREPORT_DIR)test.info
ifeq ($(COS),Darwin)
	mv *.gcno *.gcda $(CREPORT_DIR)
endif
else
	$(error You must install/activate lcov)
endif

open:
	$(COPEN) $(CREPORT)

#Qt commands
install:
	$(CINSTALL)

uninstall:
	rm -rf $(CBUILD_DIR).qmake.stash $(CBUILD_DIR)*.o $(CBUILD_DIR)*.h $(CBUILD_DIR)*.cpp $(CBUILD_DIR)Makefile
	rm -rf $(CAPP_PATH)$(CAPP_NAME) $(CBUILD_DIR)Viewer_v1_0.app/

dist: clean
	tar -cz -f $(CARCHIVE_DIR)$(CARCHIVE_NAME) $(CPROJECT_DIR)*.*

dvi:
	$(COPEN) $(CMANUAL)

run:
	./$(CAPP_PATH)$(CAPP_NAME)

#Style commands
style: refactor checkStyle

refactor:
	clang-format -i -style=Google $(CSOURCE_BACKEND)*.c $(CHEADERS_BACKEND)*.h
	clang-format -i -style=Google $(CSOURCE_FRONTEND)*.cpp $(CHEADERS_FRONTEND)*.h $(CTEST)

checkStyle:
	clang-format -n -style=Google $(CSOURCE_BACKEND)*.c $(CHEADERS_BACKEND)*.h
	clang-format -n -style=Google $(CSOURCE_FRONTEND)*.cpp $(CHEADERS_FRONTEND)*.h $(CTEST)

#Git commands
push:
	git add . && git commit -m'$(com)' && git push origin $(CBRANCH)


#Doxygen commands
doxy_build:
	cd $(CGUIDE_DIR) && doxygen Doxyfile && cd ..

doxy_rebuild: clean doxy_build

#Profile commands
leak: leak_test leak_program

leak_test: clean tests
ifeq ($(COS),Linux)
ifeq ($(shell which valgrind),/usr/bin/valgrind)
	$(CLEAK) --log-file=$(CLEAK_LOG) ./$(CRUNNER)
else
	$(error You must install valgrind to check leaks)
endif
else
	$(CLEAK) ./$(CRUNNER)
endif

leak_program: clean install
ifeq ($(COS),Linux)
ifeq ($(shell which valgrind),/usr/bin/valgrind)
	$(CLEAK) --log-file=$(CLOG_NAME) ./$(CAPP_PATH)$(CAPP_NAME)
else
	$(error You must install valgrind to check leaks)
endif
else
	$(CLEAK) ./$(CAPP_PATH)$(CAPP_NAME)
endif


profile: clean tests
ifeq ($(shell perf --version | grep -o perf),perf)
	perf stat --output=$(CLOGS_DIR)profile.log -e cache-misses,branch-misses,cycles -- taskset --cpu-list 0 ./$(CAPP_PATH)$(CAPP_NAME)
else
	$(error "You must install perf before check program")
endif

record_profile: clean install
ifeq ($(shell perf --version | grep -o perf),perf)
	perf record -e cache-misses,branch-misses,cycles -- taskset --cpu-list 0 ./$(CAPP_PATH)$(CAPP_NAME)
else
	$(error "You must install perf before check program")
endif