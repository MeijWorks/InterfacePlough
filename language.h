/*
  Config file for ploegbesturing
Copyright (C) 2011-2013 J.A. Woltjer.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

// ---------------
// Serial messages
// ---------------
#define S_MEIJWORKS     "MeijWorks"
#define S_DEVICE        "Plantercontrol v 1.00"
#define S_LIBRARY       "FarmGPS library v "
#define S_COPYRIGHT     "(c) 2011 - 2014 by J.A. Woltjer"
#define S_TIMES         "Times started: "
#define S_DIVIDE        "-------------------------------"

#define NEDERLANDS

// -----------
// Taal ENGELS
// -----------
#ifdef ENGLISH

#define L_BLANK         "                    "

#define L_MEIJWORKS     "     MeijWorks      "
#define L_DEVICE        " Plantercontrol 1.00"
#define L_COPYRIGHT     "      (c) 2014      "
#define L_AUTHOR        "  by J.A. Woltjer   "

#define L_POS           "Set width:          "
#define L_A_POS         "Actual position:    "
#define L_XTE           "XTE:                "
#define L_ROTATION      "Rotation:           "

#define L_CAL_ACCEPT    "+ : accept          "
#define L_CAL_DECLINE   "- : cancel          "

#define L_CAL_DONE      "complete            "
#define L_CAL_DECLINED  "cancelled           "

#define L_CAL_ADJUST    "+ / - : to adjust   "
#define L_CAL_ENTER     "Both  : to accept   "

#define L_CAL_POS       "Width calibration   "
#define L_CAL_POS_AD    "Adjust to        cm "

#define L_CAL_ROTATION  "Rotation calibration"
#define L_CAL_ROTATION_AD "Adjust to        deg"

#define L_CAL_SHARES    "Adjust am. of shares"
#define L_CAL_SHARES_AD "Amount of shares:   "

#define L_CAL_KP        "PID adjust KP       "
#define L_CAL_KP_AD     "KP:                 "

#define L_CAL_PWM_M     "PWM adjust manual   "
#define L_CAL_PWM_M_AD  "PWM manual:         "

#define L_CAL_PWM_A     "PWM adjust auto     "
#define L_CAL_PWM_A_AD  "PWM auto:           "

#define L_CAL_MARGIN    "Adjust error margin "
#define L_CAL_MARGIN_AD "Margin :          cm"

#define L_CAL_MAXCOR    "Adj. max. correction"
#define L_CAL_MAXCOR_AD "Max. corr.:       cm"

#define L_CAL_SWAP      "Change ploughside   "
#define L_CAL_SWAP_AD   "Side:               "

#define L_CAL_QUAL      "Correct RTK ident.  "
#define L_CAL_QUAL_AD   "Quality:            "

#define L_CAL_SPEED     "Speed calibration   "
#define L_CAL_SPEED_AD  "Accelerate to 10kph "

#define L_CAL_GPS       "GPS autodetect      "
#define L_CAL_GPS_DONE  "passed              "
#define L_CAL_GPS_FAIL  "failed...           "
#define L_CAL_GPS_M1    "Check cabling and   "
#define L_CAL_GPS_M2    "nmea output         "

#define L_CAL_COMPLETE  "Finish calibration  "
#define L_CAL_NOSAVE    "Data NOT saved      "
#define L_CAL_DDONE     "done                "
#define L_CAL_SAVE      "Data saved          "

// ---------------
// Taal NEDERLANDS
// ---------------
#elif defined NEDERLANDS

#define L_BLANK         "                    "

#define L_MEIJWORKS     "     MeijWorks      "
#define L_DEVICE        " Pootm.best.  v 1.00"
#define L_COPYRIGHT     "      (c) 2014      "
#define L_AUTHOR        "  by J.A. Woltjer   "

#define L_POS           "Ploegbreedte:       "
#define L_A_POS         "Actuele positie:    "
#define L_XTE           "XTE:                "
#define L_ROTATION      "Rotatie:            "

#define L_CAL_ACCEPT    "+ : accepteren      "
#define L_CAL_DECLINE   "- : annuleren       "

#define L_CAL_DONE      "voltooid            "
#define L_CAL_DECLINED  "geannuleerd         "

#define L_CAL_ADJUST    "+ / - : verstellen  "
#define L_CAL_ENTER     "Beide : accepteren  "

#define L_CAL_POS       "Breedte calibratie  "
#define L_CAL_POS_AD    "Verstel naar     cm "

#define L_CAL_ROTATION  "Rotatie calibratie  "
#define L_CAL_ROTATION_AD "Verstel naar     deg"

#define L_CAL_SHARES    "Wijzig aant. scharen"
#define L_CAL_SHARES_AD "Aantal scharen:     "

#define L_CAL_KP        "PID wijzig KP       "
#define L_CAL_KP_AD     "KP:                 "

#define L_CAL_PWM_M     "Wijzig PWM handmatig"
#define L_CAL_PWM_M_AD  "PWM handmatig:      "

#define L_CAL_PWM_A     "Wijzig PWM automaat "
#define L_CAL_PWM_A_AD  "PWM automaat:       "

#define L_CAL_MARGIN    "Wijzig foutmarge    "
#define L_CAL_MARGIN_AD "Foutmarge :       cm"

#define L_CAL_MAXCOR    "Wijzig max correctie"
#define L_CAL_MAXCOR_AD "Max. corr.:       cm"

#define L_CAL_SWAP      "Wijzig ploegzijde   "
#define L_CAL_SWAP_AD   "Ploegt naar:        "

#define L_CAL_QUAL      "Corrigeer RTK ident."
#define L_CAL_QUAL_AD   "Quality:            "

#define L_CAL_SPEED     "Snelheids calibratie"
#define L_CAL_SPEED_AD  "Accelereer tot 10kmh"

#define L_CAL_GPS       "GPS autodetect      "
#define L_CAL_GPS_DONE  "geslaagd            "
#define L_CAL_GPS_FAIL  "mislukt...          "
#define L_CAL_GPS_M1    "Check kabels en     "
#define L_CAL_GPS_M2    "nmea output         "

#define L_CAL_COMPLETE  "Calibratie afronden "
#define L_CAL_NOSAVE    "Data NIET opgeslagen"
#define L_CAL_DDONE     "geslaagd            "
#define L_CAL_SAVE      "Data is opgeslagen  "

// ----------
// Taal Deens
// ----------
#elif defined DANSK

#define L_BLANK         "                    "

#define L_MEIJWORKS     "     MeijWorks      "
#define L_DEVICE        "  Plovstyring v0.74 "
#define L_COPYRIGHT     "      (c) 2013      "
#define L_AUTHOR        "    J.A. Woltjer    "

#define L_WIDTH         "Arbejdsbredde:      "
#define L_A_WIDTH       "Aktuelle bredde:    "
#define L_SLIP          "Slip:              %"
#define L_XTE           "XTE:                "

#define L_CAL_ACCEPT    "+ : acceptere       "
#define L_CAL_DECLINE   "- : annullere       "

#define L_CAL_DONE      "færdig              "
#define L_CAL_DECLINED  "annulleret          "

#define L_CAL_ADJUST    "+ / - : justere     "
#define L_CAL_ENTER     "Beide : acceptere   "

#define L_CAL_WIDTH     "Bredde kalibrering  "
#define L_CAL_WIDTH_AD  "Justere til       cm"

#define L_CAL_SHARES    "ændre antal plovjern"
#define L_CAL_SHARES_AD "Antal plovjern :    "

#define L_CAL_MARGIN    "ændre fejlmargen    "
#define L_CAL_MARGIN_AD "Fejlmargen :      cm"

#define L_CAL_MAXCOR    "ændre correction    "
#define L_CAL_MAXCOR_AD "Max cor:          cm"

#define L_CAL_SIDE      "Corrigeer ploegzijde"
#define L_CAL_SIDE_AD   "Ploegt nu naar:     "

#define L_CAL_QUAL      "Corrigeer RTK ident."
#define L_CAL_QUAL_AD   "Quality:            "

#define L_CAL_SPEED     "Snelheids calibratie"
#define L_CAL_SPEED_AD  "Accelereer tot 10kmh"

#define L_CAL_GPS       "GPS autodetect      "
#define L_CAL_GPS_DONE  "succesfuld          "
#define L_CAL_GPS_FAIL  "mislykket...        "
#define L_CAL_GPS_M1    "Kontrollere kabler  "
#define L_CAL_GPS_M2    "og nmea output      "

#define L_CAL_COMPLETE  "Kalibrering færdig  "
#define L_CAL_DDONE     "færdig              "
#define L_CAL_NOSAVE    "Data IKKE er gemt   "
#define L_CAL_SAVE      "Data gemt           "

#endif
