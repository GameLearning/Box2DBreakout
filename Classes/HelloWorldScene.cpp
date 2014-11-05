#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    Sprite *_ball = Sprite::create("ball.png");
    _ball->setPosition(Vec2(100, 300));
    _ball->setTag(1);
    this->addChild(_ball);
    
    _world = new b2World(b2Vec2(0.0f, 1.0f));

    b2Body * _ballbody;
    b2BodyDef ballBodyDef;
    ballBodyDef.type = b2_dynamicBody;
    ballBodyDef.userData = _ball;
    ballBodyDef.position.Set(100/PTM_RATIO,300/PTM_RATIO);
    _ballbody = _world->CreateBody(&ballBodyDef);
    
    b2CircleShape circle;
    circle.m_radius = 26.0/PTM_RATIO;
    
    b2FixtureDef ballShapeDef;
    ballShapeDef.shape = &circle;
    ballShapeDef.density = 1.0f;
    ballShapeDef.friction = 0.0f;
    ballShapeDef.restitution = 1.0f;
    _ballFixture = _ballbody->CreateFixture(&ballShapeDef);
    
    
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, 0.0f);
    b2Body *groundBody = _world->CreateBody(&groundBodyDef);
    b2EdgeShape groundShape;
    b2FixtureDef groundShapeDef;
    groundShapeDef.shape = &groundShape;
    
    groundShape.Set(b2Vec2(0,0),b2Vec2(visibleSize.width /PTM_RATIO,0));
    _bottomFixture = groundBody->CreateFixture(&groundShapeDef);
    
    groundShape.Set(b2Vec2(0,0),b2Vec2(0,visibleSize.height /PTM_RATIO));
    groundBody->CreateFixture(&groundShapeDef);
    
    groundShape.Set(b2Vec2(visibleSize.width /PTM_RATIO,0),b2Vec2(visibleSize.width /PTM_RATIO,visibleSize.height /PTM_RATIO));
    groundBody->CreateFixture(&groundShapeDef);
    
    groundShape.Set(b2Vec2(visibleSize.width /PTM_RATIO,visibleSize.height /PTM_RATIO),b2Vec2(0,visibleSize.height /PTM_RATIO));
    groundBody->CreateFixture(&groundShapeDef);
    
    
    
    _ballbody->ApplyLinearImpulse(b2Vec2(10, 10), ballBodyDef.position,false);
    
    
    Sprite* _paddle = Sprite::create("paddle.png");
    _paddle->setPosition(visibleSize.width/2,50);
    this->addChild(_paddle);
    
    b2BodyDef paddleBodyDef;
    paddleBodyDef.type = b2_dynamicBody;
    paddleBodyDef.userData = _paddle;
    paddleBodyDef.position.Set(visibleSize.width/2/PTM_RATIO, 50/PTM_RATIO);
    _paddleBody = _world->CreateBody(&paddleBodyDef);

    b2PolygonShape paddleShape;
    paddleShape.SetAsBox(_paddle->getContentSize().width/PTM_RATIO/2,
                     _paddle->getContentSize().height/PTM_RATIO/2);
    
    b2FixtureDef paddleShapeDef;
    paddleShapeDef.shape = &circle;
    paddleShapeDef.density = 10.0f;
    paddleShapeDef.friction = 0.4f;
    paddleShapeDef.restitution = 0.1f;
    _paddleFixture = _paddleBody->CreateFixture(&paddleShapeDef);

    
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(HelloWorld::onTouchCancelled, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    this->scheduleUpdate();
    return true;
}

void HelloWorld::update(float dt){
    _world->Step(dt,10,10);
    for(b2Body *b = _world->GetBodyList(); b; b=b->GetNext()) {
        if (b->GetUserData() != NULL) {
            Sprite *ballData = (Sprite *)b->GetUserData();
            
            ballData->setPosition(Vec2(b->GetPosition().x * PTM_RATIO,
                                    b->GetPosition().y * PTM_RATIO));
            ballData->setRotation(-1 * CC_RADIANS_TO_DEGREES(b->GetAngle()));
        }
    }
}

bool HelloWorld::onTouchBegan(Touch* touch, Event* event){
    
}

void HelloWorld::onTouchMoved(Touch* touch, Event* event){

}

void HelloWorld::onTouchEnded(Touch* touch, Event* event){

}

void HelloWorld::onTouchCancelled(Touch* touch, Event* event){

}