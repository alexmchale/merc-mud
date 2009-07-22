/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"



bool	check_social	args( ( CHAR_DATA *ch, char *command,
			    char *argument ) );



/*
 * Command logging types.
 */
#define LOG_NORMAL	0
#define LOG_ALWAYS	1
#define LOG_NEVER	2



/*
 * Log-all switch.
 */
bool				fLogAll		= FALSE;



/*
 * Command table.
 */
const	struct	cmd_type	cmd_table	[] =
{
    /*
     * Common movement commands.
     */
    { "north",		do_north,	POS_STANDING,	 0,  LOG_NORMAL	},
    { "east",		do_east,	POS_STANDING,	 0,  LOG_NORMAL	},
    { "south",		do_south,	POS_STANDING,	 0,  LOG_NORMAL	},
    { "west",		do_west,	POS_STANDING,	 0,  LOG_NORMAL },
    { "up",		do_up,		POS_STANDING,	 0,  LOG_NORMAL },
    { "down",		do_down,	POS_STANDING,	 0,  LOG_NORMAL },

    /*
     * Common other commands.
     * Placed here so one and two letter abbreviations work.
     */
    { "buy",		do_buy,		POS_RESTING,	 0,  LOG_NORMAL	},
    { "cast",		do_cast,	POS_FIGHTING,	 0,  LOG_NORMAL	},
    { "exits",		do_exits,	POS_RESTING,	 0,  LOG_NORMAL	},
    { "get",		do_get,		POS_RESTING,	 0,  LOG_NORMAL	},
    { "inventory",	do_inventory,	POS_DEAD,	 0,  LOG_NORMAL	},
    { "kill",		do_kill,	POS_FIGHTING,	 0,  LOG_NORMAL	},
    { "look",		do_look,	POS_RESTING,	 0,  LOG_NORMAL	},
    { "order",		do_order,	POS_RESTING,	 0,  LOG_ALWAYS	},
    { "rest",		do_rest,	POS_RESTING,	 0,  LOG_NORMAL	},
    { "stand",		do_stand,	POS_SLEEPING,	 0,  LOG_NORMAL	},
    { "tell",		do_tell,	POS_RESTING,	 0,  LOG_NORMAL	},
    { "wield",		do_wear,	POS_RESTING,	 0,  LOG_NORMAL	},
    { "wizhelp",	do_wizhelp,	POS_DEAD,	36,  LOG_NORMAL	},

    /*
     * Informational commands.
     */
    { "areas",		do_areas,	POS_DEAD,	 0,  LOG_NORMAL	},
    { "bug",		do_bug,		POS_DEAD,	 0,  LOG_NORMAL	},
    { "commands",	do_commands,	POS_DEAD,	 0,  LOG_NORMAL },
    { "compare",	do_compare,	POS_RESTING,	 0,  LOG_NORMAL },
    { "consider",	do_consider,	POS_RESTING,	 0,  LOG_NORMAL	},
    { "credits",	do_credits,	POS_DEAD,	 0,  LOG_NORMAL	},
    { "equipment",	do_equipment,	POS_DEAD,	 0,  LOG_NORMAL	},
    { "examine",	do_examine,	POS_RESTING,	 0,  LOG_NORMAL	},
    { "help",		do_help,	POS_DEAD,	 0,  LOG_NORMAL	},
    { "idea",		do_idea,	POS_DEAD,	 0,  LOG_NORMAL	},
    { "report",		do_report,	POS_DEAD,	 0,  LOG_NORMAL	},
    { "score",		do_score,	POS_DEAD,	 0,  LOG_NORMAL	},
    { "socials",	do_socials,	POS_DEAD,	 0,  LOG_NORMAL },
    { "time",		do_time,	POS_DEAD,	 0,  LOG_NORMAL	},
    { "typo",		do_typo,	POS_DEAD,	 0,  LOG_NORMAL	},
    { "weather",	do_weather,	POS_RESTING,	 0,  LOG_NORMAL	},
    { "who",		do_who,		POS_DEAD,	 0,  LOG_NORMAL	},

    /*
     * Configuration commands.
     */
    { "channels",	do_channels,	POS_DEAD,	 0,  LOG_NORMAL	},
    { "config",		do_config,	POS_DEAD,	 0,  LOG_NORMAL	},
    { "description",	do_description,	POS_DEAD,	 0,  LOG_NORMAL	},
    { "password",	do_password,	POS_DEAD,	 0,  LOG_NEVER	},
    { "title",		do_title,	POS_DEAD,	 0,  LOG_NORMAL	},
    { "wimpy",		do_wimpy,	POS_DEAD,	 0,  LOG_NORMAL	},

    /*
     * Communication commands.
     */
    { "answer",		do_answer,	POS_SLEEPING,	 0,  LOG_NORMAL },
    { "auction",	do_auction,	POS_SLEEPING,	 0,  LOG_NORMAL	},
    { "chat",		do_chat,	POS_SLEEPING,	 0,  LOG_NORMAL	},
    { ".",		do_chat,	POS_SLEEPING,	 0,  LOG_NORMAL	},
    { "emote",		do_emote,	POS_RESTING,	 0,  LOG_NORMAL	},
    { ",",		do_emote,	POS_RESTING,	 0,  LOG_NORMAL	},
    { "gtell",		do_gtell,	POS_DEAD,	 0,  LOG_NORMAL	},
    { ";",		do_gtell,	POS_DEAD,	 0,  LOG_NORMAL	},
    { "music",		do_music,	POS_SLEEPING,	 0,  LOG_NORMAL },
    { "note",		do_note,	POS_RESTING,	 0,  LOG_NORMAL	},
    { "pose",		do_pose,	POS_RESTING,	 0,  LOG_NORMAL	},
    { "question",	do_question,	POS_SLEEPING,	 0,  LOG_NORMAL },
    { "reply",		do_reply,	POS_RESTING,	 0,  LOG_NORMAL },
    { "say",		do_say,		POS_RESTING,	 0,  LOG_NORMAL	},
    { "'",		do_say,		POS_RESTING,	 0,  LOG_NORMAL	},
    { "shout",		do_shout,	POS_RESTING,	 3,  LOG_NORMAL	},
    { "yell",		do_yell,	POS_RESTING,	 0,  LOG_NORMAL	},

    /*
     * Object manipulation commands.
     */
    { "brandish",	do_brandish,	POS_RESTING,	 0,  LOG_NORMAL	},
    { "close",		do_close,	POS_RESTING,	 0,  LOG_NORMAL	},
    { "drink",		do_drink,	POS_RESTING,	 0,  LOG_NORMAL	},
    { "drop",		do_drop,	POS_RESTING,	 0,  LOG_NORMAL	},
    { "eat",		do_eat,		POS_RESTING,	 0,  LOG_NORMAL	},
    { "fill",		do_fill,	POS_RESTING,	 0,  LOG_NORMAL	},
    { "give",		do_give,	POS_RESTING,	 0,  LOG_NORMAL	},
    { "hold",		do_wear,	POS_RESTING,	 0,  LOG_NORMAL	},
    { "list",		do_list,	POS_RESTING,	 0,  LOG_NORMAL	},
    { "lock",		do_lock,	POS_RESTING,	 0,  LOG_NORMAL	},
    { "open",		do_open,	POS_RESTING,	 0,  LOG_NORMAL	},
    { "pick",		do_pick,	POS_RESTING,	 0,  LOG_NORMAL	},
    { "put",		do_put,		POS_RESTING,	 0,  LOG_NORMAL },
    { "quaff",		do_quaff,	POS_RESTING,	 0,  LOG_NORMAL	},
    { "recite",		do_recite,	POS_RESTING,	 0,  LOG_NORMAL	},
    { "remove",		do_remove,	POS_RESTING,	 0,  LOG_NORMAL	},
    { "sell",		do_sell,	POS_RESTING,	 0,  LOG_NORMAL	},
    { "take",		do_get,		POS_RESTING,	 0,  LOG_NORMAL	},
    { "sacrifice",	do_sacrifice,	POS_RESTING,	 0,  LOG_NORMAL	},
    { "unlock",		do_unlock,	POS_RESTING,	 0,  LOG_NORMAL	},
    { "value",		do_value,	POS_RESTING,	 0,  LOG_NORMAL	},
    { "wear",		do_wear,	POS_RESTING,	 0,  LOG_NORMAL	},
    { "zap",		do_zap,		POS_RESTING,	 0,  LOG_NORMAL	},

    /*
     * Combat commands.
     */
    { "backstab",	do_backstab,	POS_STANDING,	 0,  LOG_NORMAL	},
    { "bs",		do_backstab,	POS_STANDING,	 0,  LOG_NORMAL	},
    { "disarm",		do_disarm,	POS_FIGHTING,	 0,  LOG_NORMAL	},
    { "flee",		do_flee,	POS_FIGHTING,	 0,  LOG_NORMAL	},
    { "kick",		do_kick,	POS_FIGHTING,	 0,  LOG_NORMAL	},
    { "murde",		do_murde,	POS_FIGHTING,	 5,  LOG_NORMAL	},
    { "murder",		do_murder,	POS_FIGHTING,	 5,  LOG_ALWAYS	},
    { "rescue",		do_rescue,	POS_FIGHTING,	 0,  LOG_NORMAL	},

    /*
     * Miscellaneous commands.
     */
    { "follow",		do_follow,	POS_RESTING,	 0,  LOG_NORMAL	},
    { "group",		do_group,	POS_SLEEPING,	 0,  LOG_NORMAL	},
    { "hide",		do_hide,	POS_RESTING,	 0,  LOG_NORMAL	},
    { "practice",	do_practice,	POS_SLEEPING,	 0,  LOG_NORMAL	},
    { "qui",		do_qui,		POS_DEAD,	 0,  LOG_NORMAL	},
    { "quit",		do_quit,	POS_DEAD,	 0,  LOG_NORMAL	},
    { "recall",		do_recall,	POS_FIGHTING,	 0,  LOG_NORMAL	},
    { "/",		do_recall,	POS_FIGHTING,	 0,  LOG_NORMAL	},
    { "rent",		do_rent,	POS_DEAD,	 0,  LOG_NORMAL	},
    { "save",		do_save,	POS_DEAD,	 0,  LOG_NORMAL	},
    { "sleep",		do_sleep,	POS_SLEEPING,	 0,  LOG_NORMAL	},
    { "sneak",		do_sneak,	POS_STANDING,	 0,  LOG_NORMAL	},
    { "split",		do_split,	POS_RESTING,	 0,  LOG_NORMAL	},
    { "steal",		do_steal,	POS_STANDING,	 0,  LOG_NORMAL	},
    { "train",		do_train,	POS_RESTING,	 0,  LOG_NORMAL	},
    { "visible",	do_visible,	POS_SLEEPING,	 0,  LOG_NORMAL },
    { "wake",		do_wake,	POS_SLEEPING,	 0,  LOG_NORMAL	},
    { "where",		do_where,	POS_RESTING,	 0,  LOG_NORMAL	},



    /*
     * Immortal commands.
     */
    { "advance",	do_advance,	POS_DEAD,	40,  LOG_ALWAYS	},
    { "trust",		do_trust,	POS_DEAD,	40,  LOG_ALWAYS },

    { "allow",		do_allow,	POS_DEAD,	39,  LOG_ALWAYS	},
    { "ban",		do_ban,		POS_DEAD,	39,  LOG_ALWAYS	},
    { "deny",		do_deny,	POS_DEAD,	39,  LOG_ALWAYS	},
    { "disconnect",	do_disconnect,	POS_DEAD,	39,  LOG_ALWAYS	},
    { "freeze",		do_freeze,	POS_DEAD,	39,  LOG_ALWAYS	},
    { "reboo",		do_reboo,	POS_DEAD,	39,  LOG_NORMAL	},
    { "reboot",		do_reboot,	POS_DEAD,	39,  LOG_ALWAYS	},
    { "shutdow",	do_shutdow,	POS_DEAD,	39,  LOG_NORMAL	},
    { "shutdown",	do_shutdown,	POS_DEAD,	39,  LOG_ALWAYS	},
    { "users",		do_users,	POS_DEAD,	39,  LOG_NORMAL	},
    { "wizlock",	do_wizlock,	POS_DEAD,	39,  LOG_ALWAYS	},

    { "force",		do_force,	POS_DEAD,	38,  LOG_ALWAYS	},
    { "mload",		do_mload,	POS_DEAD,	38,  LOG_ALWAYS	},
    { "mset",		do_mset,	POS_DEAD,	38,  LOG_ALWAYS	},
    { "noemote",	do_noemote,	POS_DEAD,	38,  LOG_NORMAL	},
    { "notell",		do_notell,	POS_DEAD,	38,  LOG_NORMAL	},
    { "oload",		do_oload,	POS_DEAD,	38,  LOG_ALWAYS	},
    { "oset",		do_oset,	POS_DEAD,	38,  LOG_ALWAYS	},
    { "pardon",		do_pardon,	POS_DEAD,	38,  LOG_ALWAYS	},
    { "purge",		do_purge,	POS_DEAD,	38,  LOG_NORMAL	},
    { "restore",	do_restore,	POS_DEAD,	38,  LOG_ALWAYS	},
    { "rset",		do_rset,	POS_DEAD,	38,  LOG_ALWAYS	},
    { "silence",	do_silence,	POS_DEAD,	38,  LOG_NORMAL },
    { "sla",		do_sla,		POS_DEAD,	38,  LOG_NORMAL	},
    { "slay",		do_slay,	POS_DEAD,	38,  LOG_ALWAYS	},
    { "sset",		do_sset,	POS_DEAD,	38,  LOG_ALWAYS },
    { "transfer",	do_transfer,	POS_DEAD,	38,  LOG_ALWAYS	},

    { "at",		do_at,		POS_DEAD,	37,  LOG_NORMAL	},
    { "bamfin",		do_bamfin,	POS_DEAD,	37,  LOG_NORMAL	},
    { "bamfout",	do_bamfout,	POS_DEAD,	37,  LOG_NORMAL	},
    { "echo",		do_echo,	POS_DEAD,	37,  LOG_ALWAYS	},
    { "goto",		do_goto,	POS_DEAD,	37,  LOG_NORMAL	},
    { "holylight",	do_holylight,	POS_DEAD,	37,  LOG_NORMAL	},
    { "invis",		do_invis,	POS_DEAD,	37,  LOG_NORMAL	},
    { "log",		do_log,		POS_DEAD,	37,  LOG_ALWAYS	},
    { "memory",		do_memory,	POS_DEAD,	37,  LOG_NORMAL	},
    { "mfind",		do_mfind,	POS_DEAD,	37,  LOG_NORMAL },
    { "mstat",		do_mstat,	POS_DEAD,	37,  LOG_NORMAL	},
    { "mwhere",		do_mwhere,	POS_DEAD,	37,  LOG_NORMAL },
    { "ofind",		do_ofind,	POS_DEAD,	37,  LOG_NORMAL },
    { "ostat",		do_ostat,	POS_DEAD,	37,  LOG_NORMAL	},
    { "peace",		do_peace,	POS_DEAD,	37,  LOG_NORMAL	},
    { "recho",		do_recho,	POS_DEAD,	37,  LOG_ALWAYS	},
    { "return",		do_return,	POS_DEAD,	37,  LOG_NORMAL	},
    { "rstat",		do_rstat,	POS_DEAD,	37,  LOG_NORMAL	},
    { "slookup",	do_slookup,	POS_DEAD,	37,  LOG_NORMAL },
    { "snoop",		do_snoop,	POS_DEAD,	37,  LOG_NORMAL	},
    { "switch",		do_switch,	POS_DEAD,	37,  LOG_ALWAYS	},

    { "immtalk",	do_immtalk,	POS_DEAD,	36,  LOG_NORMAL	},
    { ":",		do_immtalk,	POS_DEAD,	36,  LOG_NORMAL	},

    /*
     * End of list.
     */
    { "",		0,		POS_DEAD,	 0,  LOG_NORMAL	}
};



/*
 * The social table.
 * Add new socials here.
 * Alphabetical order is not required.
 */
const	struct	social_type	social_table [] =
{
    {
	"accuse",
	"Accuse whom?",
	"$n is in an accusing mood.",
	"You look accusingly at $M.",
	"$n looks accusingly at $N.",
	"$n looks accusingly at you.",
	"You accuse yourself.",
	"$n seems to have a bad conscience."
    },

    {
	"applaud",
	"Clap, clap, clap.",
	"$n gives a round of applause.",
	"You clap at $S actions.",
	"$n claps at $N's actions.",
	"$n gives you a round of applause.  You MUST'VE done something good!",
	"You applaud at yourself.  Boy, are we conceited!",
	"$n applauds at $mself.  Boy, are we conceited!"
    },

    {
	"bark",
	"Woof!  Woof!",
	"$n barks like a dog.",
	"You bark at $M.",
	"$n barks at $N.",
	"$n barks at you.",
	"You bark at yourself.  Woof!  Woof!",
	"$n barks at $mself.  Woof!  Woof!"
    },

    {
	"beer",
	"You down a cold, frosty beer.",
	"$n downs a cold, frosty beer.",
	"You draw a cold, frosty beer for $N.",
	"$n draws a cold, frosty beer for $N.",
	"$n draws a cold, frosty beer for you.",
	"You draw yourself a beer.",
	"$n draws $mself a beer."
    },

    {
	"beg",
	"You beg the gods for mercy.",
	"The gods fall down laughing at $n's request for mercy.",
	"You desperately try to squeeze a few coins from $M.",
	"$n begs you for money.",
	"$n begs $N for a gold piece!",
	"Begging yourself for money doesn't help.",
	"$n begs himself for money."
    },

    {
	"blush",
	"Your cheeks are burning.",
	"$n blushes.",
	"You get all flustered up seeing $M.",
	"$n blushes as $e sees $N here.",
	"$n blushes as $e sees you here.  Such an effect on people!",
	"You blush at your own folly.",
	"$n blushes as $e notices $s boo-boo."
    },

    {
	"bounce",
	"BOIINNNNNNGG!",
	"$n bounces around.",
	"You bounce onto $S lap.",
	"$n bounces onto $N's lap.",
	"$n bounces onto your lap.",
	"You bounce your head like a basketball.",
	"$n plays basketball with $s head."
    },

    {
	"bow",
	"You bow deeply.",
	"$n bows deeply.",
	"You bow before $M.",
	"$n bows before $N.",
	"$n bows before you.",
	"You kiss your toes.",
	"$n folds up like a jack knife and kisses $s own toes."
    },

    {
	"burp",
	"You burp loudly.",
	"$n burps loudly.",
	"You burp loudly to $M in response.",
	"$n burps loudly in response to $N's remark.",
	"$n burps loudly in response to your remark.",
	"You burp at yourself.",
	"$n burps at $mself.  What a sick sight."
    },

    {
	"cackle",
	"You throw back your head and cackle with insane glee!",
	"$n throws back $s head and cackles with insane glee!",
	"You cackle gleefully at $N",
	"$n cackles gleefully at $N.",
	"$n cackles gleefully at you.  Better keep your distance from $m.",
	"You cackle at yourself.  Now, THAT'S strange!",
	"$n is really crazy now!  $e cackles at $mself."
    },

    {
	"chuckle",
	"You chuckle politely.",
	"$n chuckles politely.",
	"You chuckle at $S joke.",
	"$n chuckles at $N's joke.",
	"$n chuckles at your joke.",
	"You chuckle at your own joke, since no one else would.",
	"$n chuckles at $s own joke, since none of you would."
    },

    {
	"clap",
	"You clap your hands together.",
	"$n shows $s approval by clapping $s hands together.",
	"You clap at $S performance.",
	"$n claps at $N's performance.",
	"$n claps at your performance.",
	"You clap at your own performance.",
	"$n claps at $s own performance."
    },

    {
	"comb",
	"You comb your hair - perfect.",
	"$n combs $s hair, how dashing!",
	"You patiently untangle $N's hair - what a mess!",
	"$n tries patiently to untangle $N's hair.",
	"$n pulls your hair in an attempt to comb it.",
	"You pull your hair, but it will not be combed.",
	"$n tries to comb $s tangled hair."
    },

    {
	"comfort",
	"Do you feel uncomfortable?",
	NULL,
	"You comfort $M.",
	"$n comforts $N.",
	"$n comforts you.",
	"You make a vain attempt to comfort yourself.",
	"$n has no one to comfort $m but $mself."
    },

    {
	"cringe",
	"You cringe in terror.",
	"$n cringes in terror!",
	"You cringe away from $M.",
	"$n cringes away from $N in mortal terror.",
	"$n cringes away from you.",
	"I beg your pardon?",
	NULL
    },

    {
	"cry",
	"Waaaaah ...",
	"$n bursts into tears.",
	"You cry on $S shoulder.",
	"$n cries on $N's shoulder.",
	"$n cries on your shoulder.",
	"You cry to yourself.",
	"$n sobs quietly to $mself."
    },

    {
	"cuddle",
	"Whom do you feel like cuddling today?",
	NULL,
	"You cuddle $M.",
	"$n cuddles $N.",
	"$n cuddles you.",
	"You must feel very cuddly indeed ... :)",
	"$n cuddles up to $s shadow.  What a sorry sight."
    },

    {
	"curse",
	"You swear loudly for a long time.",
	"$n swears: @*&^%@*&!",
	"You swear at $M.",
	"$n swears at $N.",
	"$n swears at you!  Where are $s manners?",
	"You swear at your own mistakes.",
	"$n starts swearing at $mself.  Why don't you help?"
    },

    {
	"curtsey",
	"You curtsey to your audience.",
	"$n curtseys gracefully.",
	"You curtsey to $M.",
	"$n curtseys gracefully to $N.",
	"$n curtseys gracefully for you.",
	"You curtsey to your audience (yourself).",
	"$n curtseys to $mself, since no one is paying attention to $m."
    },

    {
	"dance",
	"Feels silly, doesn't it?",
	"$n tries to break dance, but nearly breaks $s neck!",
	"You sweep $M into a romantic waltz.",
	"$n sweeps $N into a romantic waltz.",
	"$n sweeps you into a romantic waltz.",
	"You skip and dance around by yourself.",
	"$n dances a pas-de-une."
    },

    /*
     * This one's for Baka, Penn, and Onethumb!
     */
    {
	"drool",
	"You drool on yourself.",
	"$n drools on $mself.",
	"You drool all over $N.",
	"$n drools all over $N.",
	"$n drools all over you.",
	"You drool on yourself.",
	"$n drools on $mself."
    },

    {
	"fart",
	"Where are your manners?",
	"$n lets off a real rip-roarer ... a greenish cloud envelops $n!",
	"You fart at $M.  Boy, you are sick.",
	"$n farts in $N's direction.  Better flee before $e turns to you!",
	"$n farts in your direction.  You gasp for air.",
	"You fart at yourself.  You deserve it.",
	"$n farts at $mself.  Better $m than you."
    },

    {
	"flip",
	"You flip head over heels.",
	"$n flips head over heels.",
	"You flip $M over your shoulder.",
	"$n flips $N over $s shoulder.",
	"$n flips you over $s shoulder.  Hmmmm.",
	"You tumble all over the room.",
	"$n does some nice tumbling and gymnastics."
    },

    {
	"fondle",
	"Who needs to be fondled?",
	NULL,
	"You fondly fondle $M.",
	"$n fondly fondles $N.",
	"$n fondly fondles you.",
	"You fondly fondle yourself, feels funny doesn't it ?",
	"$n fondly fondles $mself - this is going too far !!"
    },

    {
	"french",
	"Kiss whom?",
	NULL,
	"You give $N a long and passionate kiss.",
	"$n kisses $N passionately.",
	"$n gives you a long and passionate kiss.",
	"You gather yourself in your arms and try to kiss yourself.",
	"$n makes an attempt at kissing $mself."
    },

    {
	"frown",
	"What's bothering you ?",
	"$n frowns.",
	"You frown at what $E did.",
	"$n frowns at what $E did.",
	"$n frowns at what you did.",
	"You frown at yourself.  Poor baby.",
	"$n frowns at $mself.  Poor baby."
    },

    {
	"fume",
	"You grit your teeth and fume with rage.",
	"$n grits $s teeth and fumes with rage.",
	"You stare at $M, fuming.",
	"$n stares at $N, fuming with rage.",
	"$n stares at you, fuming with rage!",
	"That's right - hate yourself!",
	"$n clenches $s fists and stomps his feet, fuming with anger."
    },

    {
	"gasp",
	"You gasp in astonishment.",
	"$n gasps in astonishment.",
	"You gasp as you realize what $e did.",
	"$n gasps as $e realizes what $N did.",
	"$n gasps as $e realizes what you did.",
	"You look at yourself and gasp!",
	"$n takes one look at $mself and gasps in astonisment!"
    },

    {
	"giggle",
	"You giggle.",
	"$n giggles.",
	"You giggle in $S's presence.",
	"$n giggles at $N's actions.",
	"$n giggles at you.  Hope it's not contagious!",
	"You giggle at yourself.  You must be nervous or something.",
	"$n giggles at $mself.  $e must be nervous or something."
    },

    {
	"glare",
	"You glare at nothing in particular.",
	"$n glares around $m.",
	"You glare icily at $M.",
	"$n glares at $N.",
	"$n glares icily at you, you feel cold to your bones.",
	"You glare icily at your feet, they are suddenly very cold.",
	"$n glares at $s feet, what is bothering $m?"
    },

    {
	"grin",
	"You grin evilly.",
	"$n grins evilly.",
	"You grin evilly at $M.",
	"$n grins evilly at $N.",
	"$n grins evilly at you.  Hmmm.  Better keep your distance.",
	"You grin at yourself.  You must be getting very bad thoughts.",
	"$n grins at $mself.  You must wonder what's in $s mind."
    },

    {
	"groan",
	"You groan loudly.",
	"$n groans loudly.",
	"You groan at the sight of $M.",
	"$n groans at the sight of $N.",
	"$n groans at the sight of you.",
	"You groan as you realize what you have done.",
	"$n groans as $e realizes what $e has done."
    },

    {
	"grope",
	"Whom do you wish to grope?",
	NULL,
	"Well, what sort of noise do you expect here?",
	"$n gropes $N.",
	"$n gropes you.",
	"You grope yourself - YUCK.",
	"$n gropes $mself - YUCK."
    },

    {
	"grovel",
	"You grovel in the dirt.",
	"$n grovels in the dirt.",
	"You grovel before $M.",
	"$n grovels in the dirt before $N.",
	"$n grovels in the dirt before you.",
	"That seems a little silly to me.",
	NULL
    },

    {
	"growl",
	"Grrrrrrrrrr ...",
	"$n growls.",
	"Grrrrrrrrrr ... take that, $N!",
	"$n growls at $N.  Better leave the room before the fighting starts.",
	"$n growls at you.  Hey, two can play it that way!",
	"You growl at yourself.  Boy, do you feel bitter!",
	"$n growls at $mself.  This could get interesting..."
    },

    {
	"grumble",
	"You grumble.",
	"$n grumbles.",
	"You grumble to $M.",
	"$n grumbles to $N.",
	"$n grumbles to you.",
	"You grumble under your breath.",
	"$n grumbles under $s breath."
    },

    {
	"grunt",
	"GRNNNHTTTT.",
	"$n grunts like a pig.",
	"GRNNNHTTTT.",
	"$n grunts to $N.  What a pig!",
	"$n grunts to you.  What a pig!",
	"GRNNNHTTTT.",
	"$n grunts to nobody in particular.  What a pig!"
    },

    {
	"hand",
	"Kiss whose hand?",
	NULL,
	"You kiss $S hand.",
	"$n kisses $N's hand.  How continental!",
	"$n kisses your hand.  How continental!",
	"You kiss your own hand.",
	"$n kisses $s own hand."
    },

    {
	"hop",
	"You hop around like a little kid.",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
    },

    {
	"hug",
	"Hug whom?",
	NULL,
	"You hug $M.",
	"$n hugs $N.",
	"$n hugs you.",
	"You hug yourself.",
	"$n hugs $mself in a vain attempt to get friendship."
    },

    {
	"kiss",
	"Isn't there someone you want to kiss?",
	NULL,
	"You kiss $M.",
	"$n kisses $N.",
	"$n kisses you.",
	"All the lonely people :(",
	NULL
    },

    {
	"laugh",
	"You laugh.",
	"$n laughs.",
	"You laugh at $N mercilessly.",
	"$n laughs at $N mercilessly.",
	"$n laughs at you mercilessly.  Hmmmmph.",
	"You laugh at yourself.  I would, too.",
	"$n laughs at $mself.  Let's all join in!!!"
    },

    {
	"lick",
	"You lick your lips and smile.",
	"$n licks $s lips and smiles.",
	"You lick $M.",
	"$n licks $N.",
	"$n licks you.",
	"You lick yourself.",
	"$n licks $mself - YUCK."
    },

    {
	"love",
	"You love the whole world.",
	"$n loves everybody in the world.",
	"You tell your true feelings to $N.",
	"$n whispers softly to $N.",
	"$n whispers to you sweet words of love.",
	"Well, we already know you love yourself (lucky someone does!)",
	"$n loves $mself, can you believe it ?"
    },

    {
	"massage",
	"Massage what?  Thin air?",
	NULL,
	"You gently massage $N's shoulders.",
	"$n massages $N's shoulders.",
	"$n gently massages your shoulders.  Ahhhhhhhhhh ...",
	"You practice yoga as you try to massage yourself.",
	"$n gives a show on yoga positions, trying to massage $mself."
    },

    {
	"moan",
	"You start to moan.",
	"$n starts moaning.",
	"You moan for the loss of $m.",
	"$n moans for the loss of $N.",
	"$n moans at the sight of you.  Hmmmm.",
	"You moan at yourself.",
	"$n makes $mself moan."
    },

    {
	"nibble",
	"Nibble on whom?",
	NULL,
	"You nibble on $N's ear.",
	"$n nibbles on $N's ear.",
	"$n nibbles on your ear.",
	"You nibble on your OWN ear.",
	"$n nibbles on $s OWN ear."
    },

    {
	"nod",
	"You nod your silly head off.",
	"$n nods $s silly head off.",
	"You nod in recognition to $M.",
	"$n nods in recognition to $N.",
	"$n nods in recognition to you.  You DO know $m, right?",
	"You nod at yourself.  Are you getting senile?",
	"$n nods at $mself.  $e must be getting senile."
    },

    {
	"nudge",
	"Nudge whom?",
	NULL,
	"You nudge $M.",
	"$n nudges $N.",
	"$n nudges you.",
	"You nudge yourself, for some strange reason.",
	"$n nudges $mself, to keep $mself awake."
    },

    {
	"nuzzle",
	"Nuzzle whom?",
	NULL,
	"You nuzzle $S neck softly.",
	"$n softly nuzzles $N's neck.",
	"$n softly nuzzles your neck.",
	"I'm sorry, friend, but that's impossible.",
	NULL
    },

    {
	"pat",
	"Pat whom?",
	NULL,
	"You pat $N on $S ass.",
	"$n pats $N on $S ass.",
	"$n pats you on your ass.",
	"You pat yourself on your ass, very sensual.",
	"$n pats $mself on the ass."
    },

    {
	"point",
	"Point at whom?",
	NULL,
	"You point at $M accusingly.",
	"$n points at $N accusingly.",
	"$n points at you accusingly.",
	"You point proudly at yourself.",
	"$n points proudly at $mself."
    },

    {
	"poke",
	"Poke whom?",
	NULL,
	"You poke $M in the ribs.",
	"$n pokes $N in the ribs.",
	"$n pokes you in the ribs.",
	"You poke yourself in the ribs, feeling very silly.",
	"$n pokes $mself in the ribs, looking very sheepish."
    },

    {
	"ponder",
	"You ponder the question.",
	"$n sits down and thinks deeply.",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
    },

    {
	"pout",
	"Ah, don't take it so hard.",
	"$n pouts.",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
    },

    {
	"pray",
	"You feel righteous, and maybe a little foolish.",
	"$n begs and grovels to the powers that be.",
	"You crawl in the dust before $M.",
	"$n falls down and grovels in the dirt before $N.",
	"$n kisses the dirt at your feet.",
	"Talk about narcissism ...",
	"$n mumbles a prayer to $mself."
    },

    {
	"puke",
	"You puke ... chunks everywhere!",
	"$n pukes.",
	"You puke on $M.",
	"$n pukes on $N.",
	"$n spews vomit and pukes all over your clothing!",
	"You puke on yourself.",
	"$n pukes on $s clothes."
    },

    {
	"punch",
	"Punch whom?",
	NULL,
	"You punch $M playfully.",
	"$n punches $N playfully.",
	"$n punches you playfully.  OUCH!",
	"You punch yourself.  You deserve it.",
	"$n punches $mself.  Why don't you join in?"
    },

    {
	"purr",
	"MMMMEEEEEEEEOOOOOOOOOWWWWWWWWWWWW.",
	"$n purrs contentedly.",
	"You purr contentedly in $M lap.",
	"$n purrs contentedly in $N's lap.",
	"$n purrs contentedly in your lap.",
	"You purr at yourself.",
	"$n purrs at $mself.  Must be a cat thing."
    },

    {
	"ruffle",
	"You've got to ruffle SOMEONE.",
	NULL,
	"You ruffle $N's hair playfully.",
	"$n ruffles $N's hair playfully.",
	"$n ruffles your hair playfully.",
	"You ruffle your hair.",
	"$n ruffles $s hair."
    },

    {
	"scream",
	"ARRRRRRRRRRGH!!!!!",
	"$n screams loudly!",
	"ARRRRRRRRRRGH!!!!!  Yes, it MUST have been $S fault!!!",
	"$n screams loudly at $N.  Better leave before $n blames you, too!!!",
	"$n screams at you!  That's not nice!  *sniff*",
	"You scream at yourself.  Yes, that's ONE way of relieving tension!",
	"$n screams loudly at $mself!  Is there a full moon up?"
    },

    {
	"shake",
	"You shake your head.",
	"$n shakes $s head.",
	"You shake $S hand.",
	"$n shakes $N's hand.",
	"$n shakes your hand.",
	"You are shaken by yourself.",
	"$n shakes and quivers like a bowl full of jelly."
    },

    {
	"shiver",
	"Brrrrrrrrr.",
	"$n shivers uncomfortably.",
	"You shiver at the thought of fighting $M.",
	"$n shivers at the thought of fighting $N.",
	"$n shivers at the suicidal thought of fighting you.",
	"You shiver to yourself?",
	"$n scares $mself to shivers."
    },

    {
	"shrug",
	"You shrug.",
	"$n shrugs helplessly.",
	"You shrug in response to $s question.",
	"$n shrugs in response to $N's question.",
	"$n shrugs in respopnse to your question.",
	"You shrug to yourself.",
	"$n shrugs to $mself.  What a strange person."
    },

    {
	"sigh",
	"You sigh.",
	"$n sighs loudly.",
	"You sigh as you think of $M.",
	"$n sighs at the sight of $N.",
	"$n sighs as $e thinks of you.  Touching, huh?",
	"You sigh at yourself.  You MUST be lonely.",
	"$n sighs at $mself.  What a sorry sight."
    },

    {
	"sing",
	"You raise your clear voice towards the sky.",
	"$n has begun to sing.",
	"You sing a ballad to $m.",
	"$n sings a ballad to $N.",
	"$n sings a ballad to you!  How sweet!",
	"You sing a little ditty to yourself.",
	"$n sings a little ditty to $mself."
    },

    {
	"smile",
	"You smile happily.",
	"$n smiles happily.",
	"You smile at $M.",
	"$n beams a smile at $N.",
	"$n smiles at you.",
	"You smile at yourself.",
	"$n smiles at $mself."
    },

    {
	"smirk",
	"You smirk.",
	"$n smirks.",
	"You smirk at $S saying.",
	"$n smirks at $N's saying.",
	"$n smirks at your saying.",
	"You smirk at yourself.  Okay ...",
	"$n smirks at $s own 'wisdom'."
    },

    {
	"snap",
	"PRONTO ! You snap your fingers.",
	"$n snaps $s fingers.",
	"You snap back at $M.",
	"$n snaps back at $N.",
	"$n snaps back at you!",
	"You snap yourself to attention.",
	"$n snaps $mself to attention."
    },

    {
	"snarl",
	"You grizzle your teeth and look mean.",
	"$n snarls angrily.",
	"You snarl at $M.",
	"$n snarls at $N.",
	"$n snarls at you, for some reason.",
	"You snarl at yourself.",
	"$n snarls at $mself."
    },

    {
	"sneeze",
	"Gesundheit!",
	"$n sneezes.",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
    },

    {
	"snicker",
	"You snicker softly.",
	"$n snickers softly.",
	"You snicker with $M about your shared secret.",
	"$n snickers with $N about their shared secret.",
	"$n snickers with you about your shared secret.",
	"You snicker at your own evil thoughts.",
	"$n snickers at $s own evil thoughts."
    },

    {
	"sniff",
	"You sniff sadly. *SNIFF*",
	"$n sniffs sadly.",
	"You sniff sadly at the way $E is treating you.",
	"$n sniffs sadly at the way $N is treating $m.",
	"$n sniffs sadly at the way you are treating $m.",
	"You sniff sadly at your lost opportunities.",
	"$n sniffs sadly at $mself.  Something MUST be bothering $m."
    },

    {
	"snore",
	"Zzzzzzzzzzzzzzzzz.",
	"$n snores loudly.",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
    },

    {
	"snowball",
	"Whom do you want to throw a snowball at?",
	NULL,
	"You throw a snowball in $N's face.",
	"$n throws a snowball at $N.",
	"$n throws a snowball at you.",
	"You throw a snowball at yourself.",
	"$n throws a snowball at $mself."
    },

    {
	"snuggle",
	"Who?",
	NULL,
	"you snuggle $M.",
	"$n snuggles up to $N.",
	"$n snuggles up to you.",
	"You snuggle up, getting ready to sleep.",
	"$n snuggles up, getting ready to sleep."
    },

    {
	"spank",
	"Spank whom?",
	NULL,
	"You spank $M playfully.",
	"$n spanks $N playfully.",
	"$n spanks you playfully.  OUCH!",
	"You spank yourself.  Kinky!",
	"$n spanks $mself.  Kinky!"
    },

    {
	"squeeze",
	"Where, what, how, whom?",
	NULL,
	"You squeeze $M fondly.",
	"$n squeezes $N fondly.",
	"$n squeezes you fondly.",
	"You squeeze yourself - try to relax a little!",
	"$n squeezes $mself."
    },

    {
	"stare",
	"You stare at the sky.",
	"$n stares at the sky.",
	"You stare dreamily at $N, completely lost in $S eyes..",
	"$n stares dreamily at $N.",
	"$n stares dreamily at you, completely lost in your eyes.",
	"You stare dreamily at yourself - enough narcissism for now.",
	"$n stares dreamily at $mself - NARCISSIST!"
    },

    {
	"strut",
	"Strut your stuff.",
	"$n struts proudly.",
	"You strut to get $S attention.",
	"$n struts, hoping to get $N's attention.",
	"$n struts, hoping to get your attention.",
	"You strut to yourself, lost in your own world.",
	"$n struts to $mself, lost in $s own world."
    },

    {
	"sulk",
	"You sulk.",
	"$n sulks in the corner.",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
    },

    {
	"thank",
	"Thank you too.",
	NULL,
	"You thank $N heartily.",
	"$n thanks $N heartily.",
	"$n thanks you heartily.",
	"You thank yourself since nobody else wants to !",
	"$n thanks $mself since you won't."
    },

    {
	"tickle",
	"Whom do you want to tickle?",
	NULL,
	"You tickle $N.",
	"$n tickles $N.",
	"$n tickles you - hee hee hee.",
	"You tickle yourself, how funny!",
	"$n tickles $mself."
    },

    {
	"twiddle",
	"You patiently twiddle your thumbs.",
	"$n patiently twiddles $s thumbs.",
	"You twiddle $S ears.",
	"$n twiddles $N's ears.",
	"$n twiddles your ears.",
	"You twiddle your ears like Dumbo.",
	"$n twiddles $s own ears like Dumbo."
    },

    {
	"wave",
	"You wave.",
	"$n waves happily.",
	"You wave goodbye to $N.",
	"$n waves goodbye to $N.",
	"$n waves goodbye to you.  Have a good journey.",
	"Are you going on adventures as well?",
	"$n waves goodbye to $mself."
    },

    {
	"whistle",
	"You whistle appreciatively.",
	"$n whistles appreciatively.",
	"You whistle at the sight of $M.",
	"$n whistles at the sight of $N.",
	"$n whistles at the sight of you.",
	"You whistle a little tune to yourself.",
	"$n whistles a little tune to $mself."
    },

    {
	"wiggle",
	"Your wiggle your bottom.",
	"$n wiggles $s bottom.",
	"You wiggle your bottom toward $M.",
	"$n wiggles $s bottom toward $N.",
	"$n wiggles his bottom toward you.",
	"You wiggle about like a fish.",
	"$n wiggles about like a fish."
    },

    {
	"wince",
	"You wince.  Ouch!",
	"$n winces.  Ouch!",
	"You wince at $M.",
	"$n winces at $N.",
	"$n winces at you.",
	"You wince at yourself.  Ouch!",
	"$n winces at $mself.  Ouch!"
    },

    {
	"wink",
	"You wink suggestively.",
	"$n winks suggestively.",
	"You wink suggestively at $N.",
	"$n winks at $N.",
	"$n winks suggestively at you.",
	"You wink at yourself ?? - what are you up to ?",
	"$n winks at $mself - something strange is going on..."
    },

    {
	"yawn",
	"You must be tired.",
	"$n yawns.",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
    },

    {
	"",
	NULL, NULL, NULL, NULL, NULL, NULL, NULL
    }
};



/*
 * The main entry point for executing commands.
 * Can be recursively called from 'at', 'order', 'force'.
 */
void interpret( CHAR_DATA *ch, char *argument )
{
    char command[MAX_INPUT_LENGTH];
    char logline[MAX_INPUT_LENGTH];
    int cmd;
    int trust;
    bool found;

    /*
     * Strip leading spaces.
     */
    while ( isspace(*argument) )
	argument++;
    if ( argument[0] == '\0' )
	return;

    /*
     * No hiding.
     */
    REMOVE_BIT( ch->affected_by, AFF_HIDE );

    /*
     * Implement freeze command.
     */
    if ( !IS_NPC(ch) && IS_SET(ch->act, PLR_FREEZE) )
    {
	send_to_char( "You're totally frozen!\n\r", ch );
	return;
    }

    /*
     * Grab the command word.
     * Special parsing so ' can be a command,
     *   also no spaces needed after punctuation.
     */
    strcpy( logline, argument );
    if ( !isalpha(argument[0]) && !isdigit(argument[0]) )
    {
	command[0] = argument[0];
	command[1] = '\0';
	argument++;
	while ( isspace(*argument) )
	    argument++;
    }
    else
    {
	argument = one_argument( argument, command );
    }

    /*
     * Look for command in command table.
     */
    found = FALSE;
    trust = get_trust( ch );
    for ( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
    {
	if ( command[0] == cmd_table[cmd].name[0]
	&&   !str_prefix( command, cmd_table[cmd].name )
	&&   cmd_table[cmd].level <= trust )
	{
	    found = TRUE;
	    break;
	}
    }

    /*
     * Log and snoop.
     */
    if ( cmd_table[cmd].log == LOG_NEVER )
	strcpy( logline, "XXXXXXXX XXXXXXXX XXXXXXXX" );

    if ( ( !IS_NPC(ch) && IS_SET(ch->act, PLR_LOG) )
    ||   fLogAll
    ||   cmd_table[cmd].log == LOG_ALWAYS )
    {
	sprintf( log_buf, "Log %s: %s", ch->name, logline );
	log_string( log_buf );
    }

    if ( ch->desc != NULL && ch->desc->snoop_by != NULL )
    {
	write_to_buffer( ch->desc->snoop_by, "% ",    2 );
	write_to_buffer( ch->desc->snoop_by, logline, 0 );
	write_to_buffer( ch->desc->snoop_by, "\n\r",  2 );
    }

    if ( !found )
    {
	/*
	 * Look for command in socials table.
	 */
	if ( !check_social( ch, command, argument ) )
	    send_to_char( "Huh?\n\r", ch );
	return;
    }

    /*
     * Character not in position for command?
     */
    if ( ch->position < cmd_table[cmd].position )
    {
	switch( ch->position )
	{
	case POS_DEAD:
	    send_to_char( "Lie still; you are DEAD.\n\r", ch );
	    break;

	case POS_MORTAL:
	case POS_INCAP:
	    send_to_char( "You are hurt far too bad for that.\n\r", ch );
	    break;

	case POS_STUNNED:
	    send_to_char( "You are too stunned to do that.\n\r", ch );
	    break;

	case POS_SLEEPING:
	    send_to_char( "In your dreams, or what?\n\r", ch );
	    break;

	case POS_RESTING:
	    send_to_char( "Nah... You feel too relaxed...\n\r", ch);
	    break;

	case POS_FIGHTING:
	    send_to_char( "No way!  You are still fighting!\n\r", ch);
	    break;

	}
	return;
    }

    /*
     * Dispatch the command.
     */
    (*cmd_table[cmd].do_fun) ( ch, argument );

    tail_chain( );
    return;
}



bool check_social( CHAR_DATA *ch, char *command, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int cmd;
    bool found;

    found  = FALSE;
    for ( cmd = 0; social_table[cmd].name[0] != '\0'; cmd++ )
    {
	if ( command[0] == social_table[cmd].name[0]
	&&   !str_prefix( command, social_table[cmd].name ) )
	{
	    found = TRUE;
	    break;
	}
    }

    if ( !found )
	return FALSE;

    if ( !IS_NPC(ch) && IS_SET(ch->act, PLR_NO_EMOTE) )
    {
	send_to_char( "You are anti-social!\n\r", ch );
	return TRUE;
    }

    switch ( ch->position )
    {
    case POS_DEAD:
	send_to_char( "Lie still; you are DEAD.\n\r", ch );
	return TRUE;

    case POS_INCAP:
    case POS_MORTAL:
	send_to_char( "You are hurt far too bad for that.\n\r", ch );
	return TRUE;

    case POS_STUNNED:
	send_to_char( "You are too stunned to do that.\n\r", ch );
	return TRUE;

    case POS_SLEEPING:
	/*
	 * I just know this is the path to a 12" 'if' statement.  :(
	 * But two players asked for it already!  -- Furey
	 */
	if ( !str_cmp( social_table[cmd].name, "snore" ) )
	    break;
	send_to_char( "In your dreams, or what?\n\r", ch );
	return TRUE;

    }

    one_argument( argument, arg );
    victim = NULL;
    if ( arg[0] == '\0' )
    {
	act( social_table[cmd].others_no_arg, ch, NULL, victim, TO_ROOM    );
	act( social_table[cmd].char_no_arg,   ch, NULL, victim, TO_CHAR    );
    }
    else if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
    }
    else if ( victim == ch )
    {
	act( social_table[cmd].others_auto,   ch, NULL, victim, TO_ROOM    );
	act( social_table[cmd].char_auto,     ch, NULL, victim, TO_CHAR    );
    }
    else
    {
	act( social_table[cmd].others_found,  ch, NULL, victim, TO_NOTVICT );
	act( social_table[cmd].char_found,    ch, NULL, victim, TO_CHAR    );
	act( social_table[cmd].vict_found,    ch, NULL, victim, TO_VICT    );

	if ( !IS_NPC(ch) && IS_NPC(victim)
	&&   !IS_AFFECTED(victim, AFF_CHARM)
	&&   IS_AWAKE(victim) )
	{
	    switch ( number_bits( 4 ) )
	    {
	    case 0:
		multi_hit( victim, ch, TYPE_UNDEFINED );
		break;

	    case 1: case 2: case 3: case 4:
	    case 5: case 6: case 7: case 8:
		act( social_table[cmd].others_found,
		    victim, NULL, ch, TO_NOTVICT );
		act( social_table[cmd].char_found,
		    victim, NULL, ch, TO_CHAR    );
		act( social_table[cmd].vict_found,
		    victim, NULL, ch, TO_VICT    );
		break;

	    case 9: case 10: case 11: case 12:
		act( "$n slaps $N.",  victim, NULL, ch, TO_NOTVICT );
		act( "You slap $N.",  victim, NULL, ch, TO_CHAR    );
		act( "$n slaps you.", victim, NULL, ch, TO_VICT    );
		break;
	    }
	}
    }

    return TRUE;
}



/*
 * Return true if an argument is completely numeric.
 */
bool is_number( char *arg )
{
    if ( *arg == '\0' )
	return FALSE;

    for ( ; *arg != '\0'; arg++ )
    {
	if ( !isdigit(*arg) )
	    return FALSE;
    }

    return TRUE;
}



/*
 * Given a string like 14.foo, return 14 and 'foo'
 */
int number_argument( char *argument, char *arg )
{
    char *pdot;
    int number;
    
    for ( pdot = argument; *pdot != '\0'; pdot++ )
    {
	if ( *pdot == '.' )
	{
	    *pdot = '\0';
	    number = atoi( argument );
	    *pdot = '.';
	    strcpy( arg, pdot+1 );
	    return number;
	}
    }

    strcpy( arg, argument );
    return 1;
}



/*
 * Pick off one argument from a string and return the rest.
 * Understands quotes.
 */
char *one_argument( char *argument, char *arg_first )
{
    char cEnd;

    while ( isspace(*argument) )
	argument++;

    cEnd = ' ';
    if ( *argument == '\'' || *argument == '"' )
	cEnd = *argument++;

    while ( *argument != '\0' )
    {
	if ( *argument == cEnd )
	{
	    argument++;
	    break;
	}
	*arg_first = LOWER(*argument);
	arg_first++;
	argument++;
    }
    *arg_first = '\0';

    while ( isspace(*argument) )
	argument++;

    return argument;
}
