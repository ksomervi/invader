#ifndef INVADER_RESOURCE_H
#define INVADER_RESOURCE_H

class resource {
  private:

  public:
    enum rtype { NONE, PLAYER, AUDIO, TEXTURE, FONT, DISPLAY };
    resource();
    resource(rtype, const char*);
    virtual ~resource() = 0;

    rtype type();
    void type(rtype); //one of bitmap, sound, game object

    bool load(rtype, const char*);
    bool load(rtype, string&);

};//end class resource

#endif //!defined(INVADER_RESOUCE_H)
