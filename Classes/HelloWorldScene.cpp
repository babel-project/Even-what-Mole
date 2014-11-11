#include "HelloWorldScene.h"
#include "network/HttpRequest.h"
#include "network/HttpClient.h"
#include "network/HttpResponse.h"
#include "cocos2d.h"

using namespace cocos2d::network;

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
//    Point origin = Director::getInstance()->getVisibleOrigin();

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("sprite.png");

    // position the sprite on the center of the screen
    sprite->setPosition(
        Point(
            visibleSize.width / 2,
            visibleSize.height / 2
        )
    );
    sprite->setTag(1);

    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);

    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    
    this->getExternalImage("http://pic.prepics-cdn.com/masatosakai/24650907.jpeg");
    
    return true;
}

bool HelloWorld::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
    auto location = touch->getLocation();
    auto sprite = this->getChildByTag(1);
    
    sprite->setPosition(location);
    
    return true;
}

void HelloWorld::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
    
}

void HelloWorld::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{

}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void HelloWorld::onHttpRequestCompleted(HttpClient* sender, HttpResponse* response)
{
    if ( ! response->isSucceed() ) {
        return;
    }
    std::vector<char> *buffer = response->getResponseData();
    Image* image = new Image();
    image->initWithImageData(reinterpret_cast<unsigned char*>(&(buffer->front())), buffer->size());
    Texture2D* texture = new Texture2D();
    texture->initWithImage(image);
    Sprite* sprite = Sprite::createWithTexture(texture);
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Size size = sprite->getContentSize();
    sprite->setPosition(Vec2((visibleSize.width + origin.x)/2,
                             (visibleSize.height + origin.y)/2));
    this->addChild(sprite, 100);
    image->release();
    texture->release();
}

void HelloWorld::getExternalImage(char const *url)
{
    HttpRequest* request = new HttpRequest();
    request->setUrl(url);
    request->setRequestType(HttpRequest::Type::GET);
    request->setResponseCallback( CC_CALLBACK_2(HelloWorld::onHttpRequestCompleted, this) );
    HttpClient::getInstance()->send(request);
    request->release();
}