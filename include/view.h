#ifndef VIEW_H
#define VIEW_H

class View {
  private:

  public:
    virtual ~View();
    virtual void setup()=0;
    virtual void loop()=0;
};

#endif
