from typing import TypedDict, List
from bs4 import BeautifulSoup
from urllib.parse import urljoin
import hashlib
import json
import os
import re
import urllib.request
import wikitextparser as wtp

def main():
    dates = []
    dates += get_gw2_festival_dates("Lunar New Year", get_wikitext_source("https://wiki.guildwars2.com/wiki/Lunar_New_Year"))
    dates += get_gw2_festival_dates("Super Adventure Festival", get_wikitext_source("https://wiki.guildwars2.com/wiki/Super_Adventure_Festival"))
    dates += get_gw2_festival_dates("Dragon Bash", get_wikitext_source("https://wiki.guildwars2.com/wiki/Dragon_Bash"))
    dates += get_gw2_festival_dates("Festival of the Four Winds", get_wikitext_source("https://wiki.guildwars2.com/wiki/Festival_of_the_Four_Winds"))
    dates += get_gw2_festival_dates("Haloween", get_wikitext_source("https://wiki.guildwars2.com/wiki/Halloween"))
    dates += get_gw2_festival_dates("Wintersday", get_wikitext_source("https://wiki.guildwars2.com/wiki/Wintersday"))

    print(json.dumps(dates))


class FestivalDate(TypedDict):
    festival: str
    name: str
    start: str
    end: str

def get_gw2_festival_dates(festival: str, wikitext: str) -> List[FestivalDate]:
    # Parse the wikitext
    parsed_wikitext = wtp.parse(wikitext)

    # Find the "Releases" section
    releases_section = None
    for section in parsed_wikitext.sections:

        if section.title is not None and section.title.strip() == 'Releases':
            releases_section = section
            break

    if releases_section is None:
        raise LookupError("Cannot find a 'Releases' section to parse festival dates.")


    festival_dates: List[FestivalDate] = []

    # Find the list within the "Releases" section and return each element
    releases_list = releases_section.get_lists()[0]
    for item in releases_list.items:
        parsed_line = wtp.parse(item)
        plain_text = parsed_line.plain_text()

        match = re.match(r"(?P<name>.*)\((?P<start>[0-9]{4}-[0-9]{2}-[0-9]{2}) — (?P<end>[0-9]{4}-[0-9]{2}-[0-9]{2})\)", plain_text)

        if match is None:
            raise ValueError("Regex did not match: " + plain_text)

        groups = match.groupdict()

        festival_dates.append({
            "festival": festival,
            "name": groups["name"].strip(),
            "start": groups["start"],
            "end": groups["end"],
        })

    return festival_dates


################################################################################
# get_wikitext_source
#
# Extracts the mediawiki source of the page
################################################################################
def get_wikitext_source(page_url: str) -> str:
    edit_page_url = get_edit_page_url(page_url)

    html = get_page(edit_page_url)
    soup = BeautifulSoup(html, features='html.parser')

    elements = soup.findAll("textarea", id="wpTextbox1")

    if len(elements) == 1:
        return elements[0].text

    raise LookupError("Cannot find the wiki page source")


################################################################################
# get_edit_page_url
#
# Searches for an edit page link on the page url presented. Some pages wont have
# edit but will instead have "view source" in this case that url is returned instead.
################################################################################
def get_edit_page_url(page_url: str) -> str:
    html = get_page(page_url)
    soup = BeautifulSoup(html, features='html.parser')

    # Attempt to find a single element with the id "ca-edit"
    elements = soup.findAll(id="ca-edit")
    if len(elements) == 1:
        if elements[0].name == "a":
            return urljoin(page_url, elements[0].attrs["href"])

        links = elements[0].findAll('a', href=re.compile(r'.*action=edit.*'))
        if len(links) == 1:
            return urljoin(page_url, links[0].attrs["href"])

    # Attempt to find a single element with the id "ca-viewsource"
    elements = soup.findAll(id="ca-viewsource")
    if len(elements) == 1:
        if elements[0].name == "a":
            return urljoin(page_url, elements[0].attrs["href"])

        links = elements[0].findAll('a', href=re.compile(r'.*action=edit.*'))
        if len(links) == 1:
            return urljoin(page_url, links[0].attrs["href"])

    raise LookupError("Cannot find an edit page link")


################################################################################
# get_page
#
# A simple wrapper to provide uniform html request headers and a simple but
# optional caching layer to prevent from hitting endpoints repeatedly while
# testing.
################################################################################
def get_page(page_url: str, cache_page: bool = False) -> str:
    haslib_sha = hashlib.new('sha256')
    haslib_sha.update(page_url.encode())
    sha = haslib_sha.hexdigest()

    cached_page_location = os.path.join("cached_pages", sha+".json")
    if os.path.exists(cached_page_location):
        with open(cached_page_location) as f:
            return json.load(f)["html"]

    headers = { 'User-Agent' : 'Mozilla/5.0' }
    html = urllib.request.urlopen(urllib.request.Request(page_url, None, headers)).read().decode("utf-8")

    if cache_page:
        with open(cached_page_location, 'w') as f:
            json.dump({"url": page_url, "html": html}, f)

    return html


if __name__ == "__main__":
    main()