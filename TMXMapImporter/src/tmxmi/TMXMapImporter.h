#pragma once

#include "tmxmi_VS\stdafx.h"
#include "tinyxml\tinystr.h"
#include "tinyxml\tinyxml.h"
#include "sssf\game\Game.h"
#include "sssf\gsm\state\GameStateManager.h"
#include "xmlfi\XMLFileImporter.h"
#include <unordered_map>

// XML TEXT
static const string		MAP_ELEMENT				= "map";
static const string		ORIENTATION_ATT			= "orientation";
static const string		ORTHOGONAL_VAL			= "orthogonal";
static const string		ISOMETRIC_VAL			= "isometric";
static const string		ID_ATTXML				= "id";
static const string		WIDTH_ATT				= "width";
static const string		HEIGHT_ATT				= "height";
static const string		TILEWIDTH_ATT			= "tilewidth";
static const string		TILEHEIGHT_ATT			= "tileheight";
static const string		IMAGEWIDTH_ATT			= "imagewidth";
static const string		IMAGEHEIGHT_ATT			= "imageheight";
static const string		TILESET_ELEMENT			= "tileset";
static const string		FIRSTGID_ATT			= "firstgid";
static const string		NAME_ATT				= "name";
static const string		IMAGE_ELEMENT			= "image";
static const string		SOURCE_ATT				= "source";
static const string		IMAGELAYER_ELEMENT		= "imagelayer";
static const string		LAYER_ELEMENT			= "layer";
static const string		DATA_ELEMENT			= "data";
static const string		PROPERTIES_ELEMENT		= "properties";
static const string		PROPERTY_ELEMENT		= "property";
static const string		COLLIDABLE_ATT			= "collidable";
static const string		PLAYER_ATT				= "player";
static const string		VALUE_ATT				= "value";
static const string		TILE_ELEMENT			= "tile";
static const string		GID_ATT					= "gid";
static const string		spawn					= "spawn";
static const string		playerID				= "player";
static const string		pounceBotID				= "pounceCat";
static const string		propellorBotID			= "propellorCat";

using std::unordered_map;

enum MapType {ORTHOGONAL_MAP, ISOMETRIC_MAP};

struct TileInfo {
	unordered_map<string, string> properties;
};

struct TileSetInfo {
	string name;
	string sourceImage;
	int firstgid;
	uint16_t tilewidth;
	uint16_t tileheight;
	uint16_t sourceImageWidth;
	uint16_t sourceImageHeight;
	map<size_t, TileInfo> tileInfo;
	unordered_map<string, string> properties;
};

struct TiledLayerInfo {
	string name;
	vector<size_t> gids;
	TileSetInfo* tileSetInfo;
	uint16_t width;
	uint16_t height;
	bool collidable;
	unordered_map<string, string> properties;
};

struct ImageLayerInfo {
	string name;
	string imageSource;
	uint16_t imagewidth;
	uint16_t imageheight;
	bool collidable;
	unordered_map<string, string> properties;
};

class TMXMapImporter : public XMLFileImporter
{
protected:
	MapType mapType;
	int width;
	int height;
	int tileWidth;
	int tileHeight;
	map<string, TileSetInfo> tileSetInfos;
	map<string, TiledLayerInfo> tiledLayerInfos;
	map<string, ImageLayerInfo> imageLayerInfos;
	unordered_map<string, string> custom;
	wstring dir;
	wstring mapLevelFilePath;

public:
	TMXMapImporter() {}
	~TMXMapImporter(){}
	bool loadWorld(Game *game, wstring initDir, wstring mapLevelFileName);

	// THESE ARE ALL THE HELPER METHODS
private:
	bool buildWorldFromInfo(Game *game);
	bool loadMapInfo();
	void loadTileSetInfo(const TiXmlNode *node);
	void loadTiledLayerInfo(const TiXmlNode *node);
	void loadImageLayerInfo(const TiXmlNode *node);
	bool loadOrthographicMap(const TiXmlElement *pElem);
	bool loadIsometricMap(const TiXmlElement *pElem);
	TileSetInfo* getTileSetForId(int id);
};