#include "tmxmi_VS\stdafx.h"
#include "tmxmi\TMXMapImporter.h"
#include "tinyxml\tinystr.h"
#include "tinyxml\tinyxml.h"
#include "sssf\game\Game.h"
#include "sssf\gsm\world\TiledLayer.h"
#include "sssf\gsm\world\Tile.h"
#include "sssf\gsm\state\GameStateManager.h"
#include "sssf\gsm\ai\bots\PounceBot.h"
#include "xmlfi\XMLFileImporter.h"
#include "Box2D\Box2D.h"

bool TMXMapImporter::loadWorld(Game *game, wstring initDir, wstring mapLevelFileName)
{
	dir = initDir;
	mapLevelFilePath = dir + mapLevelFileName;
	bool success = loadMapInfo();
	if (!success) return false;
	success = buildWorldFromInfo(game);
	if (!success) return false;
	return true;
}

bool TMXMapImporter::loadMapInfo()
{
	// LOAD THE XML DOC
	const char *charPath = newCharArrayFromWstring(mapLevelFilePath);
	TiXmlDocument doc(charPath);
	delete charPath;
	bool loadOkay = doc.LoadFile();
	if (loadOkay)
	{
		TiXmlElement *pElem = doc.FirstChildElement();
		string eName;
		if (pElem)
		{
			eName = pElem->Value();
			if (strcmp(eName.c_str(), MAP_ELEMENT.c_str()) == 0)
			{
				// WHAT'S THE TYPE OF MAP?
				const char* orientation = pElem->Attribute(ORIENTATION_ATT.c_str());
				if (strcmp(orientation, ORTHOGONAL_VAL.c_str()) == 0)
				{
					mapType = MapType::ORTHOGONAL_MAP;
					return loadOrthographicMap(pElem);
				}
				else
				{
					mapType = MapType::ISOMETRIC_MAP;
					return loadIsometricMap(pElem);
				}
			}
			else
			{
				return false;
			}
		}
	}
	else
	{
		return false;
	}
	return true;
}



bool TMXMapImporter::loadOrthographicMap(const TiXmlElement *pElem)
{
	this->width = extractIntAtt(pElem, WIDTH_ATT);
	this->height = extractIntAtt(pElem, HEIGHT_ATT);
	this->tileWidth = extractIntAtt(pElem, TILEWIDTH_ATT);
	this->tileHeight = extractIntAtt(pElem, TILEHEIGHT_ATT);

	const TiXmlNode *node = pElem->FirstChild();
	while (node)
	{
		string eName = node->Value();
		if (strcmp(eName.c_str(), TILESET_ELEMENT.c_str()) == 0)
		{
			loadTileSetInfo(node);
		}
		else if (strcmp(eName.c_str(), IMAGELAYER_ELEMENT.c_str()) == 0)
		{
			loadImageLayerInfo(node);
		}
		else if (strcmp(eName.c_str(), LAYER_ELEMENT.c_str()) == 0)
		{
			loadTiledLayerInfo(node);
		}
		else if (eName == PROPERTIES_ELEMENT) { // <properties>
			for (const TiXmlElement* p = node->FirstChildElement(); p; p = p->NextSiblingElement()) {
				if (strcmp(p->Value(), PROPERTY_ELEMENT.c_str()) == 0) { // <property>
					this->custom[p->Attribute(NAME_ATT.c_str())] = p->Attribute(VALUE_ATT.c_str());
				}
			}
		}
		node = node->NextSibling();
	}
	return true;
}

void TMXMapImporter::loadImageLayerInfo(const TiXmlNode *node)
{
	const TiXmlElement *element = node->ToElement();
	ImageLayerInfo imageLayerInfo;
	imageLayerInfo.name = extractCharAtt(element, NAME_ATT);
	// WE DON'T ACTUALLY CARE ABOUT THE TMX WIDTH AND HEIGHT FOR THIS

	// NOW GET THE IMAGE ELEMENT
	const TiXmlNode *childNode = node->FirstChild();
	imageLayerInfo.imageSource = extractCharAtt(childNode->ToElement(), SOURCE_ATT);

	// AND NOW GET IMAGE LAYER PROPERTIES
	childNode = childNode->ToElement()->NextSibling();
	if (node != NULL)
	{
		string eName = childNode->Value();
		if (strcmp(eName.c_str(), PROPERTIES_ELEMENT.c_str()) == 0)
		{
			const TiXmlNode *grandchildNode = childNode->FirstChild();
			while (grandchildNode != NULL)
			{
				const TiXmlElement *grandchildElement = grandchildNode->ToElement();
				string att = grandchildElement->Attribute(NAME_ATT.c_str());
				if (strcmp(att.c_str(), COLLIDABLE_ATT.c_str()) == 0)
				{
 					imageLayerInfo.collidable = extractBoolAtt(grandchildNode->ToElement(), VALUE_ATT);
				}
				else if (strcmp(att.c_str(), IMAGEHEIGHT_ATT.c_str()) == 0)
				{
					imageLayerInfo.imageheight= extractIntAtt(grandchildNode->ToElement(), VALUE_ATT);
				}
				else if (strcmp(att.c_str(), IMAGEWIDTH_ATT.c_str()) == 0)
				{
					imageLayerInfo.imagewidth = extractIntAtt(grandchildNode->ToElement(), VALUE_ATT);
				}
				else { // This is a custom property
					imageLayerInfo.properties[att] = grandchildElement->Attribute(VALUE_ATT.c_str());
				}
				grandchildNode = grandchildNode->NextSibling();
			}
		}
	}
	// OK, NOW LOAD THE LAYER
	imageLayerInfos[imageLayerInfo.name] = imageLayerInfo;
}



void TMXMapImporter::loadTiledLayerInfo(const TiXmlNode *node)
{
	const TiXmlElement *element = node->ToElement();
	TiledLayerInfo tiledLayerInfo;
	tiledLayerInfo.name = extractCharAtt(element, NAME_ATT);
	tiledLayerInfo.width = extractIntAtt(element, WIDTH_ATT);
	tiledLayerInfo.height = extractIntAtt(element, HEIGHT_ATT);

	// NOW GET THE PROPERTIES
	const TiXmlNode *propertiesNode = node->FirstChild();
	string eName = propertiesNode->Value();
	const TiXmlNode *dataNode;
	if (strcmp(eName.c_str(), PROPERTIES_ELEMENT.c_str()) == 0)
	{
		const TiXmlNode *propNode = propertiesNode->FirstChild();
		while (propNode != NULL)
		{
			const TiXmlElement *propElement = propNode->ToElement();
			string att = propElement->Attribute(NAME_ATT.c_str());
			if (strcmp(att.c_str(), COLLIDABLE_ATT.c_str()) == 0)
			{
				tiledLayerInfo.collidable = extractBoolAtt(propNode->ToElement(), VALUE_ATT);
			}
			else {
				tiledLayerInfo.properties[att] = propElement->Attribute(VALUE_ATT.c_str());
			}
			propNode = propNode->NextSibling();
		}
		dataNode = propertiesNode->NextSibling();
	}

	// NOW GET THE TILE DATA
	if (dataNode == NULL)
	{
		dataNode = node->FirstChild();
	}
	const TiXmlNode *tileNode = dataNode->FirstChild();
	while (tileNode)
	{
		const TiXmlElement *element = tileNode->ToElement();
		int gid = extractIntAtt(element, GID_ATT);
		tiledLayerInfo.gids.push_back(gid);
		tileNode = tileNode->NextSibling();
	}
	tiledLayerInfo.tileSetInfo = getTileSetForId(tiledLayerInfo.gids[0]);
	tiledLayerInfos[tiledLayerInfo.name] = tiledLayerInfo;
}

void TMXMapImporter::loadTileSetInfo(const TiXmlNode *node)
{
	const TiXmlElement *element = node->ToElement();

	TileSetInfo tileSetInfo;
	tileSetInfo.firstgid = extractIntAtt(element, FIRSTGID_ATT);
	tileSetInfo.name = extractCharAtt(element, NAME_ATT);
	tileSetInfo.tilewidth = extractIntAtt(element, TILEWIDTH_ATT);
	tileSetInfo.tileheight = extractIntAtt(element, TILEHEIGHT_ATT);

	// NOW GET THE IMAGE INFO
	element = node->FirstChildElement(IMAGE_ELEMENT.c_str());
	tileSetInfo.sourceImage = extractCharAtt(element, SOURCE_ATT);
	tileSetInfo.sourceImageWidth = extractIntAtt(element, WIDTH_ATT);
	tileSetInfo.sourceImageHeight = extractIntAtt(element, HEIGHT_ATT);

	// Rows and columns in the TILESET, not the level
	int cols = tileSetInfo.sourceImageWidth / tileSetInfo.tilewidth;
	int rows = tileSetInfo.sourceImageHeight / tileSetInfo.tileheight;
	vector<bool> tilesAdded(rows * cols, false);

	for (const TiXmlElement* t = node->FirstChildElement(TILE_ELEMENT.c_str()); t; t = t->NextSiblingElement()) {
		if (strcmp(t->Value(), TILE_ELEMENT.c_str()) == 0) {
			// If this is really a <tile> element...
			const TiXmlElement* tileProps = t->FirstChildElement(PROPERTIES_ELEMENT.c_str());
			TileInfo tileInfo;

			for (const TiXmlElement* p = tileProps->FirstChildElement(); p; p = p->NextSiblingElement()) {
				if (strcmp(p->Value(), PROPERTY_ELEMENT.c_str()) == 0) {
					// If this is really a <property> element...
					tileInfo.properties[p->Attribute(NAME_ATT.c_str())] = p->Attribute(VALUE_ATT.c_str());
				}
			}

			int id = std::stoi(t->Attribute(ID_ATTXML.c_str()));
			tilesAdded[id] = true;
			tileSetInfo.tileInfo[id] = tileInfo;
		}
	}

	for (size_t i = 0; i < tilesAdded.size(); ++i) {
		if (!tilesAdded[i]) {
			tileSetInfo.tileInfo[i] = TileInfo();
		}
	}

	const TiXmlElement* properties = node->FirstChildElement(PROPERTIES_ELEMENT.c_str());
	if (properties) {
		for (const TiXmlElement* p = properties->FirstChildElement(); p; p = p->NextSiblingElement()) {
			if (strcmp(p->Value(), PROPERTY_ELEMENT.c_str()) == 0) {
				// If this is really a <property> element...
				tileSetInfo.properties[p->Attribute(NAME_ATT.c_str())] = p->Attribute(VALUE_ATT.c_str());
			}
		}
	}

	tileSetInfos[tileSetInfo.name] = tileSetInfo;
}

bool TMXMapImporter::loadIsometricMap(const TiXmlElement *pElem)
{
	return true;
}

bool TMXMapImporter::buildWorldFromInfo(Game *game)
{
	TextureManager *worldTextureManager = game->getGraphics()->getWorldTextureManager();
	b2Body* tile;
	b2BodyDef tileDef;
	b2FixtureDef tileFixDef;
	b2PolygonShape tileShape;
	b2Vec2 tilePos;
	tileShape.SetAsBox(0.5f, 0.5f);
	tileFixDef.shape = &tileShape;
	tileDef.type = b2_staticBody;
	if (mapType == MapType::ORTHOGONAL_MAP)
	{
		World *world = game->getGSM()->getWorld();
		int largestLayerWidth = 0;
		int largestLayerHeight = 0;

		// LET'S FIRST FIGURE OUT THE WORLD WIDTH AND HEIGHT

		// FIRST THE IMAGE LAYERS
		map<string, ImageLayerInfo>::const_iterator iliIt = imageLayerInfos.begin();
		while (iliIt != imageLayerInfos.end())
		{
			string key = iliIt->first;
			ImageLayerInfo ili = imageLayerInfos[key];
			if (ili.imagewidth > largestLayerWidth)
				largestLayerWidth = ili.imagewidth;
			if (ili.imageheight > largestLayerHeight)
				largestLayerHeight = ili.imageheight;

			iliIt++;
		}
		// AND THE TILED LAYERS
		map<string, TiledLayerInfo>::const_iterator tliIt = tiledLayerInfos.begin();
		while (tliIt != tiledLayerInfos.end())
		{
			string key = tliIt->first;
			TiledLayerInfo tli = tiledLayerInfos[key];
			int layerWidth = tli.width * tli.tileSetInfo->tilewidth;
			if (layerWidth > largestLayerWidth)
				largestLayerWidth = layerWidth;
			int layerHeight = tli.height * tli.tileSetInfo->tileheight;
			if (layerHeight > largestLayerHeight)
				largestLayerHeight = layerHeight;
			tliIt++;
		}
		unsigned int idOffset = worldTextureManager->getWStringTable()->getNumWStringsInTable();

		// FIRST LOAD ALL THE TILE SETS
		map<string, TileSetInfo>::const_iterator tsiIt = tileSetInfos.begin();
		while (tsiIt != tileSetInfos.end())
		{
			string key = tsiIt->first;
			TileSetInfo tsi = tileSetInfos[key];
			wstring sourceImageW(tsi.sourceImage.begin(), tsi.sourceImage.end());
			bool success = worldTextureManager->loadTileSetFromTexture(game, dir, sourceImageW, tsi.tilewidth, tsi.tileheight);
			if (!success) return false;
			tsiIt++;
		}

		// NOW LOAD THE IMAGE LAYERS, IF THERE ARE ANY
		iliIt = imageLayerInfos.begin();
		while (iliIt != imageLayerInfos.end())
		{
			string key = iliIt->first;
			ImageLayerInfo ili = imageLayerInfos[key];
			
			TiledLayer *imageLayerToAdd = new TiledLayer(	1,
				1,
				ili.imagewidth,
				ili.imageheight,
				0,
				ili.collidable,
				largestLayerWidth,
				largestLayerHeight);
			world->addLayer(imageLayerToAdd);

			Tile *imageTile = new Tile();
			imageTile->collidable = ili.collidable;
			wstring imageSourceW(ili.imageSource.begin(), ili.imageSource.end());
			imageTile->textureID = worldTextureManager->loadTexture(dir + imageSourceW);
			imageLayerToAdd->addTile(imageTile);
	
			iliIt++;
		}

		// AND NOW LOAD THE TILED LAYERS, WHICH REFERENCE THE TILE SETS
		tliIt = tiledLayerInfos.begin();
		while (tliIt != tiledLayerInfos.end())
		{
			// @TODO WE'LL NEED TO CUSTOMIZE THIS
			bool collidableLayer = false;
			string key = tliIt->first;
			TiledLayerInfo tli = tiledLayerInfos[key];
			TiledLayer *tiledLayerToAdd = new TiledLayer(	tli.width,
															tli.height,
															tli.tileSetInfo->tilewidth,
															tli.tileSetInfo->tileheight,
															0,
															tli.collidable,
															largestLayerWidth,
															largestLayerHeight);
			world->addLayer(tiledLayerToAdd);

			// WE HAVE TO ADD ALL THE TILES
			TileSetInfo* tileSet = tli.tileSetInfo;

			int row = 0;
			int col = 0;
			int uncollidableIndex = tli.tileSetInfo->firstgid;
			for (unsigned int i = 0; i < tli.gids.size(); i++)
			{
				Tile *tileToAdd = new Tile();
				
				tileToAdd->textureID = tli.gids[i] + idOffset - 1;
				if (tli.gids[i] == uncollidableIndex){
					tileToAdd->collidable = false;
				}
				else{
					tileToAdd->collidable = tli.collidable;
				}
				tileToAdd->properties = tileSet->tileInfo[tileToAdd->textureID].properties;

				if (tileToAdd->properties[spawn] != ""){
					tileToAdd->collidable = false;
					tileToAdd->textureID = idOffset - 1;
				}

				tiledLayerToAdd->addTile(tileToAdd);
			}

			//NOW ADD ALL COLLIDABLE TILES TO THE BOX2D SIMULATION
			b2Body* tile;
			b2BodyDef tileDef;
			b2FixtureDef tileFixDef;
			b2PolygonShape tileShape;
			b2Vec2 tilePos;
			tileShape.SetAsBox(0.5f, 0.5f);
			tileFixDef.shape = &tileShape;
			tileDef.type = b2_staticBody;
			tileDef.fixedRotation = true;
			row = 0;
			col = 0;

			//Tile by tile, NOT GOOD ENOUGH!
			/*while (row < tiledLayerToAdd->getRows()){
				while (col < tiledLayerToAdd->getColumns()){
					if (tiledLayerToAdd->getTile(row, col)->collidable){
						tilePos.Set(col-0.5f, tiledLayerToAdd->getRows()-row+0.5f);
						tileDef.position = tilePos;
						tile = game->getGSM()->getPhysics()->getWorld()->CreateBody(&tileDef);
						tile->CreateFixture(&tileFixDef);
					}
					col++;
				}
				row++;
				col = 0;
			}*/

			//WHAZAM! COMBINES CONTIGUOUS HROIZONTAL TILES INTO ONE LARGE PLANE!
			//I realized vertical should be done too so the Super Mario Bros 1 wall jump glitch doesn't happen.
			//Side note, the Super Mario Bros 1 wall jump glitch is probably caused by the same problem that's solved here. Cool.
			//Doing both at the same time would be exceedingly confusing, so I'm splitting it up into two parts. Here is horizontal:
			int collidableCount = 0;
			while (row < tiledLayerToAdd->getRows()){
				while (col < tiledLayerToAdd->getColumns()){
					if (tiledLayerToAdd->getTile(row, col)->collidable){
						collidableCount++;
					}
					else{
						if (collidableCount > 1){
							tilePos.Set((col - 0.5f)-((collidableCount+1)*0.5f), tiledLayerToAdd->getRows() - row + 0.5f);
							tileDef.position = tilePos;
							tileShape.SetAsBox(0.5f*(collidableCount), 0.5f);
							tile = game->getGSM()->getPhysics()->getWorld()->CreateBody(&tileDef);
							tile->CreateFixture(&tileFixDef);
						}
						collidableCount = 0;
					}
					col++;
				}
				if (collidableCount > 1){
					tilePos.Set((col - 0.5f) - ((collidableCount+1)*0.5f), tiledLayerToAdd->getRows() - row + 0.5f);
					tileDef.position = tilePos;
					tileShape.SetAsBox(0.5f*(collidableCount), 0.5f);
					tile = game->getGSM()->getPhysics()->getWorld()->CreateBody(&tileDef);
					tile->CreateFixture(&tileFixDef);
				}
				collidableCount = 0;
				row++;
				col = 0;
			}

			//Now the vertical contiguous tiles...
			//A consequence of this is actually that we can't have any singular tiles not connected to any other
			//That is, a tile MUST touch at least one other to exist. A single collidable tile should probably never exist anyway.
			//The fix is really messy, so let's see if we can avoid that for now.
			collidableCount = 0;
			row = 0;
			col = 0;
			while (col < tiledLayerToAdd->getColumns()){
				while (row < tiledLayerToAdd->getRows()){
					if (tiledLayerToAdd->getTile(row, col)->collidable){
						collidableCount++;
					}
					else{
						if (collidableCount > 1){
							tilePos.Set(col - 0.5f, (tiledLayerToAdd->getRows() - row + 0.5f) + (collidableCount)*0.5f + 0.5f);
							tileDef.position = tilePos;
							tileShape.SetAsBox(0.5f, 0.5f*(collidableCount));
							tile = game->getGSM()->getPhysics()->getWorld()->CreateBody(&tileDef);
							tile->CreateFixture(&tileFixDef);
						}
						collidableCount = 0;
					}
					row++;
				}
				if (collidableCount > 1){
					tilePos.Set(col - 0.5f, (tiledLayerToAdd->getRows() - row + 0.5f) + (collidableCount)*0.5f + 0.5f);
					tileDef.position = tilePos;
					tileShape.SetAsBox(0.5f, 0.5f*(collidableCount));
					tile = game->getGSM()->getPhysics()->getWorld()->CreateBody(&tileDef);
					tile->CreateFixture(&tileFixDef);
				}
				collidableCount = 0;
				col++;
				row = 0;
			}


			//HERE IS WHERE WE INTERPRET ALL OF THE SPAWN TILES!
			row = 0;
			col = 0;
			while (row < tiledLayerToAdd->getRows()){
				while (col < tiledLayerToAdd->getColumns()){
					if (tiledLayerToAdd->getTile(row, col)->properties[spawn] == playerID){
						AnimatedSprite *player = game->getGSM()->getSpriteManager()->getPlayer();

						//Right here is what I think making the character's box should look like
						//Then I started wonderning how the hell we're going to do rendering
						//when we handle it by pixel and now everything's done in METERS
						//WHY DOES IT HAVE TO BE METERS? I stopped here.

						//^ That problem was solved. The things being shown to a player are actually
						//just a projection of the simulation, the actual simulation isn't tied to
						//rendering like I thought it was.
						b2BodyDef playerProps;
						playerProps.position.Set(col - 0.5f, tiledLayerToAdd->getRows() - row + 0.5f);
						playerProps.type = b2_dynamicBody;
						playerProps.fixedRotation = true;
						player->setBody(game->getGSM()->getPhysics()->getWorld()->CreateBody(&playerProps));
						b2FixtureDef fixtureDef;
						b2PolygonShape shape;
						/*This solution made the main character an octagon, he didn't get snagged anymore,
						but he also ramped off of random pieces of ground... no fun*/
						//Actually, this octagon shape is better for jumping up on higher platforms.
						//The octagonal shape gives a more natural curve to the hitbox, leading to less frustration.
						//Also gives a nice little push forward if you fall of a ledge.
						float32 width = 0.60f;
						float32 height = 1.0f;
						float32 edgeWidth = 0.1f;
						float32 edgeHeight = 0.2f;
						b2Vec2 vertices[8];
						vertices[0].Set(-width + edgeWidth, -height);		// bottom
						vertices[1].Set(width - edgeWidth, -height);		// bottom-right edge start
						vertices[2].Set(width, -height + edgeHeight);		// bottom-right edge end
						vertices[3].Set(width, height - edgeHeight);		// top-right edge start
						vertices[4].Set(width - edgeWidth, height);			// top-right edge end
						vertices[5].Set(-width + edgeWidth, height);		// top-left edge start
						vertices[6].Set(-width, height - edgeHeight);		// top-left edge end
						vertices[7].Set(-width, -height + edgeHeight);		// bottom-left edge
						shape.Set(vertices, 8);

						//I like rectangles anyway
						//shape.SetAsBox(0.7, 1);
						fixtureDef.shape = &shape;
						player->getBody()->CreateFixture(&fixtureDef);

						player->setIsPlayer(true);
						player->setHP(3);
						player->setControllable(true);

						player->setHurtBox(NULL);

						b2BodyDef hurtBoxProps;
						b2Vec2 playerPos = player->getBody()->GetPosition();
						hurtBoxProps.fixedRotation = true;
						fixtureDef.isSensor = true;

						shape.SetAsBox(0.5f, 0.5f);
						fixtureDef.shape = &shape;
						fixtureDef.isSensor = true;
						if (player->isFacingRight()){
							hurtBoxProps.position.Set(playerPos.x + 0.9f, playerPos.y - 0.25f);
						}
						else{
							hurtBoxProps.position.Set(playerPos.x - 0.9f, playerPos.y - 0.25f);
						}

						player->setHurtBox(game->getGSM()->getPhysics()->getWorld()->CreateBody(&hurtBoxProps));
						player->getHurtBox()->CreateFixture(&fixtureDef);
						player->getHurtBox()->SetUserData(player);
						player->getHurtBox()->SetActive(false);

						player->getBody()->SetSleepingAllowed(false);

						//For collision detection, tells the player's body to point back at the player
						player->getBody()->SetUserData(player);

						player->setAlpha(255);
						player->setCurrentState(L"JUMPING_DESCEND_RIGHT");
						player->setFacingRight(true);
						player->setAirborne(true);
					}
					else if (tiledLayerToAdd->getTile(row, col)->properties[spawn] == pounceBotID){
						PounceBot *pounceBot = new PounceBot();

						b2BodyDef pounceBotProps;
						b2FixtureDef fixtureDef;
						b2PolygonShape shape;

						pounceBotProps.position.Set(col - 0.5f, tiledLayerToAdd->getRows() - row + 0.5f);
						pounceBotProps.type = b2_dynamicBody;
						pounceBotProps.fixedRotation = true;
						shape.SetAsBox(0.7f, 0.4f);
						fixtureDef.shape = &shape;

						pounceBot->setBody(game->getGSM()->getPhysics()->getWorld()->CreateBody(&pounceBotProps));
						pounceBot->getBody()->CreateFixture(&fixtureDef);
						pounceBot->getBody()->SetUserData(pounceBot);
						pounceBot->getBody()->SetSleepingAllowed(false);
						pounceBot->setFacingRight(false);
						pounceBot->setAirborne(true);
						pounceBot->setAlpha(255);
						pounceBot->setCurrentState(L"IDLE_LEFT");
						game->getGSM()->getSpriteManager()->addBot(pounceBot);
					}
					else if (tiledLayerToAdd->getTile(row, col)->properties[spawn] == propellorBotID){

					}
					col++;
				}
				row++;
				col = 0;
			}

			tliIt++;
		}

		// AND MAKE THE WORLD DIMENSIONS THE
		// SIZE OF THE LARGEST LAYER
		world->setWorldWidth(largestLayerWidth);
		world->setWorldHeight(largestLayerHeight);
	}
	return true;
}

TileSetInfo* TMXMapImporter::getTileSetForId(int id)
{
	map<string, TileSetInfo>::const_iterator it = tileSetInfos.begin();
	while (it != tileSetInfos.end())
	{
		string key = it->first;
		TileSetInfo tsi = tileSetInfos[key];
		int columns = tsi.sourceImageWidth/tsi.tilewidth;
		int rows = tsi.sourceImageHeight/tsi.tileheight;
		int numTiles = rows * columns;
		if ((id >= tsi.firstgid) && (id < (tsi.firstgid + numTiles)))
		{
			return &tileSetInfos[key];
		}
		it++;
	}
	return NULL;
}