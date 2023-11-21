# circular_buf

Simple circular buffer, buffer size can only be a power of 2 so a bit mask can be used to wrap back to the first element with no math.

## All the circular buffer code is in the the header file at: ./src/circular_buf.h

A lot of the rest is me trying out unit testing and github CI/CD. Although I only got as far as getting make test working. For the Docker and github part, I didn't get any further than copy and pasting some files I thought might make a starting point.

### All this extra testing etc. code is here because this is a very simple lib and is all C/C++ code that can run and be tested on any computer without needing any specifics hardware like most of my code for the Arduino, RP2040 etc, would

* [x] After installing CppUTest in the ./tools/cpputest directory by hand running "make test" from the circular_buf base directory it now runs the test in ./test/circular_buf/tests.cpp

* [ ] Get The Dockerfile changed to work with this project.
* [ ] Get the testing part of the CI/CD pipeline working with github actions.
* [ ] Get the deployment/versions number updates working with github actions.
* [ ] Maybe get github: issues pull request, merge working.
