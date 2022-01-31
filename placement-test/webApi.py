import urllib.request, json
from urllib.error import HTTPError

class ampifyPackParser(object):
    def __init__(self,pathName):
        try:
            with urllib.request.urlopen(pathName) as url:
                self.rawData = json.loads(url.read().decode())
                self.packData = self.rawData['packs']
        except HTTPError as err:
            if err.code == 404:
                print("404 Error. Ampify packs URL not found.")
                return
            else:
                raise

    def prettyPrintObject(self, obj):
        print("Pack ID: " + obj['id'])
        print("Itunes ID: " + obj['itunesId'])
        print("Date Added: " + obj['dateAdded'])
        print("Pack Name: " + obj['name'])
        print("Genres:")
        print(obj['genres'], sep="\n")
        print("\n")

    def getGenres(self):
        allGenresDup = []
        for pack in self.packData:
            for g in pack['genres']:
                allGenresDup.append(g)
        allGenresNonDup = set(allGenresDup)
        return allGenresNonDup


    def getPacksOfGenre(self, genre):
        output = []
        for pack in self.packData:
            inPack = [True for g in pack['genres'] if g == genre]
            if inPack:
                output.append(pack)

        if len(output) == 0:
            print("There were no packs of this genre.")
        else:
            for obj in output:
                print("\n")
                self.prettyPrintObject(obj)



if __name__ == "__main__":

    aP = ampifyPackParser("https://api.ampifymusic.com/packs")
    genres = aP.getGenres()
    print(genres)
    aP.getPacksOfGenre('hip-hop')
