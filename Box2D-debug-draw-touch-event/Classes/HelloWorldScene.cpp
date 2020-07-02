

#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "GLES-Render.h"
#include "DebugLayer.h"
#include "MyContactListener.h"
#include "../cocos2d/external/Box2D/include/Box2D/Dynamics/b2Body.h"
#include "../cocos2d/external/Box2D/include/Box2D/Dynamics/b2Fixture.h"
#include "../cocos2d/external/bullet/include/bullet/BulletDynamics/Character/btKinematicCharacterController.h"
#include "../cocos2d/cocos/math/Vec2.h"

#define PTM_RATIO 32.0
#define FLOOR_HEIGHT 62.0f

USING_NS_CC;


Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object

    return HelloWorld::create();
    //return scene;
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label

    //m_debugDrawLayer = new B2DebugDrawLayer();

    //auto background = DrawNode::create();
    //background->drawSolidRect(origin, visibleSize, Color4F(0.6,0.6,0.6,1.0));
    //this->addChild(background, -1);

    b2Vec2 gravity;
    gravity.Set(0.0f, -10.0f);
    m_world = new b2World(gravity);
    m_world->SetContinuousPhysics(true);

    m_debugLayer = new DebugLayer(m_world);
    this->addChild(m_debugLayer, 9999);

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0, 0);
    groundBody = m_world->CreateBody(&groundBodyDef);

    CCPoint myPosition = getPosition();
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();

    float widthInMeters = screenSize.width*1.0f / PTM_RATIO;
    float maxHeightInMeters = screenSize.height / PTM_RATIO;
    float minHeightInMeters = FLOOR_HEIGHT / PTM_RATIO;
    b2Vec2 lowerLeftCorner = b2Vec2(0, minHeightInMeters);
    b2Vec2 lowerRightCorner = b2Vec2(widthInMeters, minHeightInMeters);
    b2Vec2 upperLeftCorner = b2Vec2(0, maxHeightInMeters);
    b2Vec2 upperRightCorner = b2Vec2(widthInMeters, maxHeightInMeters);

    // Define the static container body, which will provide the collisions at screen borders.
    b2BodyDef screenBorderDef;
    screenBorderDef.position.Set(0, 0);
    b2Body* screenBorderBody = m_world->CreateBody(&screenBorderDef);
    b2EdgeShape screenBorderShape;

    // Create fixtures for the four borders (the border shape is re-used)
    screenBorderShape.Set(lowerLeftCorner, lowerRightCorner);
    screenBorderBody->CreateFixture(&screenBorderShape, 0);
    screenBorderShape.Set(lowerRightCorner, upperRightCorner);
    screenBorderBody->CreateFixture(&screenBorderShape, 0);
    screenBorderShape.Set(upperRightCorner, upperLeftCorner);
    screenBorderBody->CreateFixture(&screenBorderShape, 0);
    screenBorderShape.Set(upperLeftCorner, lowerLeftCorner);
    screenBorderBody->CreateFixture(&screenBorderShape, 0);


    initTouch();

    b2CircleShape circleShape;
    circleShape.m_p.Set(0, 0); //position, relative to body position
    circleShape.m_radius = 26.0/PTM_RATIO;
    //circleShape.m_radius = 1; //radius
    boulder = Sprite::create("boulder.png");
    boulder->setPosition(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2);
    this->addChild(boulder);


    // Ball2 initialization
    b2BodyDef boulderdef;
    boulderdef.type = b2_dynamicBody; //this will be a dynamic body
    boulderdef.position.Set((origin.x + visibleSize.width/2)/PTM_RATIO, (origin.y + visibleSize.height/2)/PTM_RATIO); //a little to the left
    boulderdef.userData = boulder;

    //debugLayer->setPosition(30,20);
    dynamicBody = m_world->CreateBody(&boulderdef);
    //dynamicBody->SetGravityScale(1);


    boulderfixture.shape = &circleShape; //this is a pointer to the shape above
    boulderfixture.density = 1.0f;
    boulderfixture.friction = 0.0f;
    boulderfixture.restitution = 0.0f;
    dynamicBody->CreateFixture(&boulderfixture); //add a fixture to the body
    //myFixtureDef1.filter.categoryBits = (uint16)PhysicsCategory::Bomba;
    //myFixtureDef1.filter.maskBits = (uint16)PhysicsCategory::Bomba;


    schedule( schedule_selector(HelloWorld::tick) );

    return true;
}

void HelloWorld::initTouch()
{
    auto listener = EventListenerTouchOneByOne::create();
    listener -> onTouchBegan = [] (Touch* touch, Event* event) { return true;};
    listener -> onTouchMoved = CC_CALLBACK_2(HelloWorld::ccTouchesMoved, this);
    //listener -> onTouchMoved = [=] (Touch* touch, Event* event) {};
    listener -> onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchesEnded, this);
    //listener -> onTouchEnded = CC_CALLBACK_2(HelloWorld::ccTouchesMoved, this);
    _eventDispatcher -> addEventListenerWithSceneGraphPriority(listener, this);
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}


void HelloWorld::onTouchesEnded(Touch* touch, Event* evento)
{
    dynamicBody->SetLinearVelocity(b2Vec2(0,0));
}

void HelloWorld::ccTouchesMoved(Touch* touch, Event* evento)
{
    //CCLOG("************** Touch event called ***************");
    auto currPosition = touch->getLocation();
    auto director = Director::getInstance();

    //if(boulder->getBoundingBox().containsPoint(currPosition)) This line should be commented so that you can tap outside sprite area
    {
        if (touch) {
            //this->touch(locationInGLFromTouch(touch));
            CCLOG("************** Grounding event called ***************");
            //this->touch(director->convertToGL(touch->getLocationInView()));
            this->touch(currPosition);
        }
    }
}

void HelloWorld::touch(Point location)
{
    auto winSize = Director::getInstance()->getWinSize();
    b2Vec2 currentVelocity = dynamicBody->GetLinearVelocity();
    b2Vec2 impulse(0.0f,0.0f);

    //auto touchLocation = this->convertToNodeSpace(location);
    b2Vec2 touchLocation(location.x, location.y);
    b2Vec2 playerPos = dynamicBody->GetPosition();

    if (location.x < (winSize.width * 0.5f))
    {
        impulse.x = -2.0f;
        dynamicBody->ApplyLinearImpulse(impulse, dynamicBody->GetWorldCenter(), true);
    }
    else if  (location.x > (winSize.width * 0.5f))
    {
        impulse.x = 2.0f;
        dynamicBody->ApplyLinearImpulse(impulse, dynamicBody->GetWorldCenter(), true);

    }

/*
    uint diffX = location.x - playerPos.x;
    if (diffX < 0) // touch towards left of sprite, so move left
    {
        impulse.x = -1.0f;
        dynamicBody->ApplyLinearImpulse(impulse, dynamicBody->GetWorldCenter(), true);
        CCLOG("**************** diffX less than 0 = %u***********************", diffX);
    }
    else if (diffX > 0)
    {
        impulse.x = 1.0f;
        dynamicBody->ApplyLinearImpulse(impulse, dynamicBody->GetWorldCenter(), true);
        CCLOG("**************** diffX GREATER than 0 = %u ***********************", diffX);
        CCLOG("Player position = %u, touch location = %u",playerPos.x, touchLocation.x);
    }
*/
}

void HelloWorld::tick(float dt)
{
    m_world->Step(dt, 10, 10);
    for(b2Body *b = m_world->GetBodyList(); b; b=b->GetNext()) {
        if (b->GetUserData() != NULL) {
            CCSprite *ballData = (CCSprite *)b->GetUserData();
            ballData->setPosition( CCPointMake( b->GetPosition().x * PTM_RATIO, b->GetPosition().y * PTM_RATIO) );
            ballData->setRotation( -1 * CC_RADIANS_TO_DEGREES(b->GetAngle()) );
        }
    }
}
