#include "q3utils.h"

int get_result(int picture, int statue, int giftshop) {
    int Picture = 0, Statue =1, GiftShop = 2;
    if (picture > giftshop) {
        if (picture >= statue) return Picture;                   // picture has higher priority than statue 
        else return Statue;                                      // Statue has the highest ranking so we choose it
    } else if (picture == giftshop) {
        if (picture < statue) return Statue;                     // Statue has the highest ranking so we choose it
        else return GiftShop;                                    // gift shop has highest priority
    } else {                                                     // gift shop > picture case
        if (statue > giftshop) return Statue;                    // Statue has the highest ranking so we choose it
        else return GiftShop;                                    // gift shop has the highest priotity
    }
}