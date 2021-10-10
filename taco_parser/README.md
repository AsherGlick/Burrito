# Introduction

A very simple util that parses the XML and corresponding C struct trail files (using nom) from Taco (GW2 overlay), to produce a json compatible format for [Burrito](https://github.com/AsherGlick/Burrito), a GW2 overlay for Linux.

Note that a syntehtic xml file is included in `/data/` for unit testing.

No `.trl` file is included in the repo.

## Usage

```
let json_str = taco_to_json_str("folder_name", "test.xml");
```
