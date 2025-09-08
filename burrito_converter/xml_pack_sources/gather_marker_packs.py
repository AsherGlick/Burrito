import re
import yaml
import requests
from dataclasses import dataclass
from typing import List, TypedDict
import json
import os

chunk_size: int = 8192


class ReleaseInfo(TypedDict):
    tag: str
    date: str
    assets: List[str]


################################################################################
# get_github_release_assets
#
# Queries the github releases API or loads previous queries from a cache to get
# a list of a repos releases and the files that are contained in them.
################################################################################
_github_release_assets_cache_path = "_github_release_assets_cache.json"
_github_release_assets_cache = None
per_page = 100
def get_github_release_assets(owner: str, repo: str) -> List[ReleaseInfo]:
    # Caching reading
    global _github_release_assets_cache
    if _github_release_assets_cache is None:
        if os.path.exists(_github_release_assets_cache_path):
            with open(_github_release_assets_cache_path, "r") as f:
                _github_release_assets_cache = json.load(f)
        else:
            _github_release_assets_cache = {}
    if owner in _github_release_assets_cache and repo in _github_release_assets_cache[owner]:
        return _github_release_assets_cache[owner][repo]

    # Query the github api on a cache miss
    api_url = f"https://api.github.com/repos/{owner}/{repo}/releases"
    
    page = 1
    
    releases: List[ReleaseInfo] = []
    while True:
        response = requests.get(api_url, headers={"Accept": "application/vnd.github.v3+json"}, params={"per_page": per_page, "page": page})
        response.raise_for_status()
        data = response.json()
        
        if not data:
            break

        for rel in data:
            # Skip prereleases
            if rel.get("prerelease"):
                continue

            releases.append({
                "tag": rel.get("tag_name"),
                "date": rel.get("published_at"),
                "assets": [asset["browser_download_url"] for asset in rel.get("assets", [])]
            })
    
        # Check if another page exists in the "Link" header
        link_header = response.headers.get("Link", "")
        if 'rel="next"' not in link_header:
            break
        
        page += 1

    # Add the new query to the cache
    if owner not in _github_release_assets_cache:
        _github_release_assets_cache[owner] = {}
    _github_release_assets_cache[owner][repo] = releases
    with open(_github_release_assets_cache_path + ".tmp", 'w') as f:
        json.dump(_github_release_assets_cache, f)
    os.rename(_github_release_assets_cache_path + ".tmp", _github_release_assets_cache_path)

    return releases


################################################################################
# download_asset
#
# Downloads an asset to a location on disk. If the directory does not exist
# then the directories are created. If the file already exists then it is not
# re-downloaded.
################################################################################
def download_asset(target_path: str, asset_url: str) -> None:
    print(target_path, asset_url)

    if os.path.exists(target_path):
        return

    # Ensure the parent directory exists
    os.makedirs(os.path.dirname(target_path), exist_ok=True)

    with requests.get(asset_url, stream=True) as r:
        r.raise_for_status()
        with open(target_path + ".tmp", "wb") as f:
            for chunk in r.iter_content(chunk_size=chunk_size):
                if chunk:  # filter out keep-alive chunks
                    f.write(chunk)
        os.rename(target_path + ".tmp", target_path)



repo_regex = re.compile(r"https://github\.com/(?P<owner>[^/]+)/(?P<repo>[^/]+)/?.*")

def main():
    with open("database.yaml", "r") as f:
        marker_pack_sources = yaml.safe_load(f)

    for marker_pack_source in marker_pack_sources:
        if marker_pack_source["update_scheme"] == "latest_github_release":

            repo_url = marker_pack_source["versions"][0]["url"]    
            match = repo_regex.match(repo_url)
            if match is None:
                raise ValueError("Could not match the github url pattern to extract user and repo")
            owner = match.groupdict()["owner"]
            repo = match.groupdict()["repo"]

            releases = get_github_release_assets(owner, repo)
            for release in releases:
                release["assets"] = [x for x in release["assets"] if x.endswith(".taco") or x.endswith(".zip") or x.endswith(".burrito") or x.endswith(".guildpoint")]
                if len(release["assets"]) > 0:
                    for asset_url in release["assets"]:
                        target_path = os.path.join("marker_packs", marker_pack_source["name"], release["tag"], os.path.basename(asset_url))
                        download_asset(target_path, asset_url)


if __name__ == "__main__":
    main()