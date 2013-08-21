/*
 * This is a collection of named colours known as the "X11 Colours",
 * https://en.wikipedia.org/wiki/X11_color_names
 *
 * The numbers shown here before the division sign are the RGB (24-bit)
 * values, divided down to become RGB565.
 *
 *
 * IMPORTANT
 * The names in this file are very common, and may be repeated elsewhere
 * in other header files. If you get errors about redefined names, simply
 * put the line
 * #define _FTOLED_Colours
 * above the line "#include <FTOLED.h>" in your sketch, and this file
 * will be ignored (none of these colours will be available, but the error
 * will go away.)
 */
#ifndef _FTOLED_Colours
#define _FTOLED_Colours
const Colour WHITE                = { 255/8, 255/4, 255/8 };
const Colour BLACK                = {   0/8,   0/4,   0/8 };
const Colour RED                  = { 255/8,   0/4,   0/8 };
const Colour GREEN                = {   0/8, 255/4,   0/8 };
const Colour BLUE                 = {   0/8,   0/4, 255/8 };
const Colour SNOW                 = { 255/8, 250/4, 250/8 };
const Colour GHOSTWHITE           = { 248/8, 248/4, 255/8 };
const Colour WHITESMOKE           = { 245/8, 245/4, 245/8 };
const Colour GAINSBORO            = { 220/8, 220/4, 220/8 };
const Colour FLORALWHITE          = { 255/8, 250/4, 240/8 };
const Colour OLDLACE              = { 253/8, 245/4, 230/8 };
const Colour LINEN                = { 250/8, 240/4, 230/8 };
const Colour ANTIQUEWHITE         = { 250/8, 235/4, 215/8 };
const Colour PAPAYAWHIP           = { 255/8, 239/4, 213/8 };
const Colour BLANCHEDALMOND       = { 255/8, 235/4, 205/8 };
const Colour BISQUE               = { 255/8, 228/4, 196/8 };
const Colour PEACHPUFF            = { 255/8, 218/4, 185/8 };
const Colour NAVAJOWHITE          = { 255/8, 222/4, 173/8 };
const Colour MOCCASIN             = { 255/8, 228/4, 181/8 };
const Colour CORNSILK             = { 255/8, 248/4, 220/8 };
const Colour IVORY                = { 255/8, 255/4, 240/8 };
const Colour LEMONCHIFFON         = { 255/8, 250/4, 205/8 };
const Colour SEASHELL             = { 255/8, 245/4, 238/8 };
const Colour HONEYDEW             = { 240/8, 255/4, 240/8 };
const Colour MINTCREAM            = { 245/8, 255/4, 250/8 };
const Colour AZURE                = { 240/8, 255/4, 255/8 };
const Colour ALICEBLUE            = { 240/8, 248/4, 255/8 };
const Colour LAVENDER             = { 230/8, 230/4, 250/8 };
const Colour LAVENDERBLUSH        = { 255/8, 240/4, 245/8 };
const Colour MISTYROSE            = { 255/8, 228/4, 225/8 };
const Colour DARKSLATEGRAY        = {  47/8,  79/4,  79/8 };
const Colour DARKSLATEGREY        = {  47/8,  79/4,  79/8 };
const Colour DIMGRAY              = { 105/8, 105/4, 105/8 };
const Colour DIMGREY              = { 105/8, 105/4, 105/8 };
const Colour SLATEGRAY            = { 112/8, 128/4, 144/8 };
const Colour SLATEGREY            = { 112/8, 128/4, 144/8 };
const Colour LIGHTSLATEGRAY       = { 119/8, 136/4, 153/8 };
const Colour LIGHTSLATEGREY       = { 119/8, 136/4, 153/8 };
const Colour GRAY                 = { 190/8, 190/4, 190/8 };
const Colour GREY                 = { 190/8, 190/4, 190/8 };
const Colour LIGHTGREY            = { 211/8, 211/4, 211/8 };
const Colour LIGHTGRAY            = { 211/8, 211/4, 211/8 };
const Colour MIDNIGHTBLUE         = {  25/8,  25/4, 112/8 };
const Colour NAVY                 = {   0/8,   0/4, 128/8 };
const Colour NAVYBLUE             = {   0/8,   0/4, 128/8 };
const Colour CORNFLOWERBLUE       = { 100/8, 149/4, 237/8 };
const Colour DARKSLATEBLUE        = {  72/8,  61/4, 139/8 };
const Colour SLATEBLUE            = { 106/8,  90/4, 205/8 };
const Colour MEDIUMSLATEBLUE      = { 123/8, 104/4, 238/8 };
const Colour LIGHTSLATEBLUE       = { 132/8, 112/4, 255/8 };
const Colour MEDIUMBLUE           = {   0/8,   0/4, 205/8 };
const Colour ROYALBLUE            = {  65/8, 105/4, 225/8 };
const Colour DODGERBLUE           = {  30/8, 144/4, 255/8 };
const Colour DEEPSKYBLUE          = {   0/8, 191/4, 255/8 };
const Colour SKYBLUE              = { 135/8, 206/4, 235/8 };
const Colour LIGHTSKYBLUE         = { 135/8, 206/4, 250/8 };
const Colour STEELBLUE            = {  70/8, 130/4, 180/8 };
const Colour LIGHTSTEELBLUE       = { 176/8, 196/4, 222/8 };
const Colour LIGHTBLUE            = { 173/8, 216/4, 230/8 };
const Colour POWDERBLUE           = { 176/8, 224/4, 230/8 };
const Colour PALETURQUOISE        = { 175/8, 238/4, 238/8 };
const Colour DARKTURQUOISE        = {   0/8, 206/4, 209/8 };
const Colour MEDIUMTURQUOISE      = {  72/8, 209/4, 204/8 };
const Colour TURQUOISE            = {  64/8, 224/4, 208/8 };
const Colour CYAN                 = {   0/8, 255/4, 255/8 };
const Colour LIGHTCYAN            = { 224/8, 255/4, 255/8 };
const Colour CADETBLUE            = {  95/8, 158/4, 160/8 };
const Colour MEDIUMAQUAMARINE     = { 102/8, 205/4, 170/8 };
const Colour AQUAMARINE           = { 127/8, 255/4, 212/8 };
const Colour DARKGREEN            = {   0/8, 100/4,   0/8 };
const Colour DARKOLIVEGREEN       = {  85/8, 107/4,  47/8 };
const Colour DARKSEAGREEN         = { 143/8, 188/4, 143/8 };
const Colour SEAGREEN             = {  46/8, 139/4,  87/8 };
const Colour MEDIUMSEAGREEN       = {  60/8, 179/4, 113/8 };
const Colour LIGHTSEAGREEN        = {  32/8, 178/4, 170/8 };
const Colour PALEGREEN            = { 152/8, 251/4, 152/8 };
const Colour SPRINGGREEN          = {   0/8, 255/4, 127/8 };
const Colour LAWNGREEN            = { 124/8, 252/4,   0/8 };
const Colour CHARTREUSE           = { 127/8, 255/4,   0/8 };
const Colour MEDIUMSPRINGGREEN    = {   0/8, 250/4, 154/8 };
const Colour GREENYELLOW          = { 173/8, 255/4,  47/8 };
const Colour LIMEGREEN            = {  50/8, 205/4,  50/8 };
const Colour YELLOWGREEN          = { 154/8, 205/4,  50/8 };
const Colour FORESTGREEN          = {  34/8, 139/4,  34/8 };
const Colour OLIVEDRAB            = { 107/8, 142/4,  35/8 };
const Colour DARKKHAKI            = { 189/8, 183/4, 107/8 };
const Colour KHAKI                = { 240/8, 230/4, 140/8 };
const Colour PALEGOLDENROD        = { 238/8, 232/4, 170/8 };
const Colour LIGHTGOLDENRODYELLOW = { 250/8, 250/4, 210/8 };
const Colour LIGHTYELLOW          = { 255/8, 255/4, 224/8 };
const Colour YELLOW               = { 255/8, 255/4,   0/8 };
const Colour GOLD                 = { 255/8, 215/4,   0/8 };
const Colour LIGHTGOLDENROD       = { 238/8, 221/4, 130/8 };
const Colour GOLDENROD            = { 218/8, 165/4,  32/8 };
const Colour DARKGOLDENROD        = { 184/8, 134/4,  11/8 };
const Colour ROSYBROWN            = { 188/8, 143/4, 143/8 };
const Colour INDIANRED            = { 205/8,  92/4,  92/8 };
const Colour SADDLEBROWN          = { 139/8,  69/4,  19/8 };
const Colour SIENNA               = { 160/8,  82/4,  45/8 };
const Colour PERU                 = { 205/8, 133/4,  63/8 };
const Colour BURLYWOOD            = { 222/8, 184/4, 135/8 };
const Colour BEIGE                = { 245/8, 245/4, 220/8 };
const Colour WHEAT                = { 245/8, 222/4, 179/8 };
const Colour SANDYBROWN           = { 244/8, 164/4,  96/8 };
const Colour TAN                  = { 210/8, 180/4, 140/8 };
const Colour CHOCOLATE            = { 210/8, 105/4,  30/8 };
const Colour FIREBRICK            = { 178/8,  34/4,  34/8 };
const Colour BROWN                = { 165/8,  42/4,  42/8 };
const Colour DARKSALMON           = { 233/8, 150/4, 122/8 };
const Colour SALMON               = { 250/8, 128/4, 114/8 };
const Colour LIGHTSALMON          = { 255/8, 160/4, 122/8 };
const Colour ORANGE               = { 255/8, 165/4,   0/8 };
const Colour DARKORANGE           = { 255/8, 140/4,   0/8 };
const Colour CORAL                = { 255/8, 127/4,  80/8 };
const Colour LIGHTCORAL           = { 240/8, 128/4, 128/8 };
const Colour TOMATO               = { 255/8,  99/4,  71/8 };
const Colour ORANGERED            = { 255/8,  69/4,   0/8 };
const Colour HOTPINK              = { 255/8, 105/4, 180/8 };
const Colour DEEPPINK             = { 255/8,  20/4, 147/8 };
const Colour LIGHTPINK            = { 255/8, 182/4, 193/8 };
const Colour PALEVIOLETRED        = { 219/8, 112/4, 147/8 };
const Colour MAROON               = { 176/8,  48/4,  96/8 };
const Colour MEDIUMVIOLETRED      = { 199/8,  21/4, 133/8 };
const Colour VIOLETRED            = { 208/8,  32/4, 144/8 };
const Colour MAGENTA              = { 255/8,   0/4, 255/8 };
const Colour VIOLET               = { 238/8, 130/4, 238/8 };
const Colour PLUM                 = { 221/8, 160/4, 221/8 };
const Colour ORCHID               = { 218/8, 112/4, 214/8 };
const Colour MEDIUMORCHID         = { 186/8,  85/4, 211/8 };
const Colour DARKORCHID           = { 153/8,  50/4, 204/8 };
const Colour DARKVIOLET           = { 148/8,   0/4, 211/8 };
const Colour BLUEVIOLET           = { 138/8,  43/4, 226/8 };
const Colour PURPLE               = { 160/8,  32/4, 240/8 };
const Colour MEDIUMPURPLE         = { 147/8, 112/4, 219/8 };
const Colour THISTLE              = { 216/8, 191/4, 216/8 };
const Colour DARKGREY             = { 169/8, 169/4, 169/8 };
const Colour DARKGRAY             = { 169/8, 169/4, 169/8 };
const Colour DARKBLUE             = { 0  /8,   0/4, 139/8 };
const Colour DARKCYAN             = { 0  /8, 139/4, 139/8 };
const Colour DARKMAGENTA          = { 139/8,   0/4, 139/8 };
const Colour DARKRED              = { 139/8,   0/4,   0/8 };
const Colour LIGHTGREEN           = { 144/8, 238/4,  14/84 };
#endif
