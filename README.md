# C++ Meetup Raffle Name Picker

A little expository C++ project for choosing raffle winners at C++ meetup events.  
It fetches the names of all the event's RSVP members and randomly chooses a winner.

The winner is selected *only* from members that RSVPed "yes" for the event.  
It does *not* take into account any `+N` guests, or non-RSVP attendees.

The code is designed to fit on a single slide (excluding HTTP/JSON libs).  
Interesting C++ features used by the code include:
- `filesystem::path`;
- `std::string_literals`;
- `std::mt19937` and `std::random_device`;
- Ranges TS-ish usage, showing both `view`s and `action`s;
- Networking TS-ish usage for fetching data

### Usage
`> meetup_raffle <meetup.com API key> <Group URL Name> [More Group URL Names]` 

If the chosen member is not present (despite the RSVP), type `more`<kbd>Enter ⏎</kbd> for another randomly chosen name.

### Building

1. Use git to clone and update the submodules;  
2. Use CMake to build, providing it with the proper Boost paths;
3. Do a short talk reviewing/auditing the application C++ code;
4. Run at your event to choose the raffle winner!

### Dependencies
- Boost.Beast from [**Boost**](boost.org): Access the Meetup REST API.  
- [**nlohmann::json**](https://github.com/nlohmann/json): For JSON processing
- [**Ranges-v3-VS2015**](https://github.com/Microsoft/Range-V3-VS2015): Because Ranges.

**Ranges** is a git submodules, **Beast** ships with **Boost** and needs to be supplied externally and the single **JSON** file `json.hpp` is already in. When Ranges are adopted into C++, they will no longer be needed as separate dependencies. 

### Contribute

The code is far from perfect and can surely be improved for both exposition and clarity.  
Contributions and pull requests will be gladly accepted:

- Support for non-MSVC/Windows builds.
- Allow event selection based on date, when there are more than one (currently chooses the first event).
- Any suggestions, remarks, ideas - please open and issue and/or make a PR.

<p align="center">🎫</p>

Finally, if you used this in a raffle, _please_ let [me](https://twitter.com/adishavit) know.   
It will make me very happy!

<p align="center">🎫</p>

### In the Wild

- [**Ben Deane**](https://twitter.com/ben_deane) presented a snippet from this code at his **"Best Presentation"** award winning [CppNow 2018](http://cppnow.org/history/2018/) presentation: [**"Easy to Use, Hard to Misuse: Declarative Style in C++"**](https://buff.ly/2lcJxcI)

