#ifndef INVADER_RESOURCE_H
#define INVADER_RESOURCE_H

class resource {
  private:

  public:
    enum rtype { NONE, PLAYER, AUDIO, TEXTURE, FONT, DISPLAY };
    resource(rtype, const char*);
    resource();
    rtype type();
    void type(rtype); //one of bitmap, sound, game object

    //basic_object * 



};//end class resource

#endif //!defined(INVADER_RESOUCE_H)
