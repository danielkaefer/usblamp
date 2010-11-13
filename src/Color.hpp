#ifndef COLOR_H
#define	COLOR_H

class Color {
public:
    Color(char red, char green, char blue);
    char getRed();
    char getGreen();
    char getBlue();
    virtual ~Color();
private:
    char red;
    char green;
    char blue;

};

#endif	/* COLOR_H */

