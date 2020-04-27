/**********************************************************************************
 * MIT License
 * 
 * Copyright (c) 2018 Antoine Beauchamp
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *********************************************************************************/

// testfilegenerator.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <time.h>   // for time()
#include <cstdlib>  // for rand(), srand()
#include <ctime>    // for rand(), srand()

#include "rapidassist/cli.h"
#include "rapidassist/logging.h"
#include "rapidassist/strings.h"
#include "rapidassist/filesystem.h"

#include "bin2cpp/version.h"

const char * getVersionString()
{
  return BIN2CPP_VERSION;
}

std::string getSizeAdditionalInfo(int size)
{
  std::string output;
  if (size > 1024)
  {
    output << " (" << ra::filesystem::GetUserFriendlySize(size) << ")";
  }
  return output;
}

void printVersion()
{
  printf("testfilegenerator v%s\n", getVersionString() );
}

void printUsage()
{
  //usage string in docopt format. See http://docopt.org/
  static const char usage[] = 
    "Usage:\n"
    "  testfilegenerator --file=<path> [--size=<value>] [--quiet] [--fill=<name>] [--seed=<value>] [--skip=<value>]\n"
    "  testfilegenerator --help\n"
    "  testfilegenerator --version\n"
    "\n"
    "Options:\n"
    "  --help               Display this help message.\n"
    "  --version            Display this application version.\n"
    "  --file=<path>        Path of the generated file.\n"
    "  --size=<value>       Size of generated file in bytes. [default: 1024]\n"
    "  --fill=<name>        Name of the filling strategy. Must be one of the following:\n"
    "                         sequential  : Insert values from 0x00 to 0xFF into the file. Default option.\n"
    "                         random      : Insert random data into the file. See 'seed' and 'skip' options.\n"
    "                         text        : Insert text data into the file.\n"
    "                         html        : Insert html data into the file.\n"
    "  --seed=<value>       Define a seed value to initialze the random number generator. Defaults to local time.\n"
    "  --skip=<value>       Define the number of random number to skip before dumping to the file. [default: 0]\n"
    "  --quiet              Do not log any message to standard output.\n"
    "\n";
  printf("%s", usage);
}

enum RETURN_CODE
{
  EXIT_NO_ERROR = 0,
  MISSING_FILE,
  INVALID_FILE_SIZE,
  INVALID_FILL_PARAMETER,
  INVALID_SEED,
  INVALID_SKIP
};

int main(int argc, char **argv)
{
  //parse arguments
  std::string file;
  int size = 1024; //in bytes
  unsigned int seed = (unsigned int)time(NULL); //warning C4244: 'initializing' : conversion from 'time_t' to 'unsigned int', possible loss of data
  unsigned int skip = 0;
  std::string fill = "sequential";

  //help
  std::string tmp;
  if (ra::cli::ParseArgument("help", tmp, argc, argv))
  {
    printVersion();
    printUsage();
    return EXIT_NO_ERROR;
  }

  //version
  if (ra::cli::ParseArgument("version", tmp, argc, argv))
  {
    printVersion();
    return EXIT_NO_ERROR;
  }

  //quiet
  std::string tmpQuiet;
  if (ra::cli::ParseArgument("quiet", tmpQuiet, argc, argv))
  {
    ra::logging::SetQuietMode(true);
  }

  //file
  if (!ra::cli::ParseArgument("file", file, argc, argv))
  {
    ra::logging::Log(ra::logging::LOG_ERROR, "Missing 'file' argument!");
    printUsage();
    return MISSING_FILE;
  }

  //size
  int tmpSize = 0;
  if (ra::cli::ParseArgument("size", tmpSize, argc, argv))
  {
    size = tmpSize;
    if (size <= 0)
    {
      ra::logging::Log(ra::logging::LOG_ERROR, "Invalid file size!");
      return INVALID_FILE_SIZE;
    }
  }

  //seed
  int tmpSeed = 0;
  if (ra::cli::ParseArgument("seed", tmpSeed, argc, argv))
  {
    seed = tmpSeed;
    if (seed < 0)
    {
      ra::logging::Log(ra::logging::LOG_ERROR, "Invalid seed value!");
      return INVALID_SEED;
    }
  }

  //skip
  int tmpSkip = 0;
  if (ra::cli::ParseArgument("skip", tmpSkip, argc, argv))
  {
    skip = tmpSkip;
    if (skip < 0)
    {
      ra::logging::Log(ra::logging::LOG_ERROR, "Invalid skip value!");
      return INVALID_SKIP;
    }
  }

  //fill
  std::string tmpFill;
  if (ra::cli::ParseArgument("fill", tmpFill, argc, argv))
  {
    if (tmpFill == "sequential" ||
        tmpFill == "random" ||
        tmpFill == "text" ||
        tmpFill == "html")
    {
      fill = tmpFill;
    }
    else
    {
      ra::logging::Log(ra::logging::LOG_ERROR, "Invalid fill parameter!");
      return INVALID_FILL_PARAMETER;
    }
  }

  //building info string
  std::string infostr;
  infostr << "Writing " << size << " bytes" << getSizeAdditionalInfo(size) << " of " << fill << " data into \'" << file << "\'.";
  ra::logging::Log(ra::logging::LOG_INFO, "%s", infostr.c_str());

  //process with file generation
  FILE * f = fopen(file.c_str(), "wb");
  if (!f)
  {
    ra::logging::Log(ra::logging::LOG_ERROR, "Cannot create file '%s'!", file.c_str());
    return 4;
  }

  //fill file according to fill strategy
  if (fill == "random")
  {
    //init random number generator
    srand( seed );

    //skip n bytes
    for(unsigned int i=0; i<skip; i++)
    {
      int dummy = rand();
    }

    //write content
    for(int i=0; i<size; i++)
    {
      const char c = (const char)(rand()%256);
      fwrite(&c, 1, 1, f);
    }
  }
  else
  {
    //build pattern
    std::string pattern;

    if (fill == "sequential")
    {
      for(int i=0; i<256; i++)
      {
        const char c = (const char)i;
        pattern.append( std::string(&c, 1) );
      }
    }
    else if (fill == "text")
    {
      //build pattern
      pattern = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Integer tempus gravida ipsum, "
                "vel venenatis metus pellentesque eu. In cursus et arcu vitae tempor. Aenean eu arcu condimentum, "
                "fringilla felis eget, accumsan quam. Mauris ut vehicula nisi. Phasellus vulputate dictum leo sit "
                "amet suscipit. In hac habitasse platea dictumst. Donec a fringilla ipsum, eu molestie nisl. "
                "Vivamus faucibus, orci vel gravida condimentum, sapien urna venenatis augue, non consectetur "
                "augue orci eget dolor.";
    }
    else if (fill == "html")
    {
      //build pattern (google.com home page)
      pattern =   "<!doctype html><html itemscope=\"\" itemtype=\"http://schema.org/WebPage\" lang=\"en-CA\"><head><meta content=\"text/html; charset=UTF-8\" http-equiv=\"Content-Type\"><meta content=\"/images/branding/googleg/1x/googleg_standard_color_128dp.png\" itemprop=\"image\"><title>Google</title><script>(function(){window.google={kEI:'p94UWrLhB4zWjwTfgp0o',kEXPI:'201794,1354276,1354688,1354723,1354915,1355218,1355735,1355771,1355892,1355923,1356031,1356079,1356184,1356347,1356631,1356696,3700276,3700440,3700489,4029815,4031109,4040138,4043492,4045841,4048347,4063220,4076999,4078430,4081038,4081164,4095910,4097153,4097470,4097922,4097929,4098721,4098728,4098752,4102238,4103475,4103861,4104258,4105241,4109316,4109489,4113215,4115697,4116349,4116724,4116731,4116926,4116935,4117328,4117539,4117980,4118227,4118303,4118798,4119032,4119034,4119036,4120415,4120660,4121035,4121518,4122382,4124090,4124729,4124850,4125837,4126200,4127445,4127744,4128232,4128378,4128586,4128624,4129001,4129555,4129633,4130560,4130782,4131072,4131247,4131834,4132"
                  "956,4133090,4133113,4133416,4134268,4134440,4134882,4134919,4134948,4135088,4135249,4135402,4135576,4135747,4135856,4135926,4135934,4136073,4136235,4137098,4137110,4137467,4137469,4137596,4137646,4138341,4138853,4139008,4139214,4139394,4139435,4139462,4139496,4139500,4139701,4140081,4140109,4140153,4140241,4140464,4140792,4140797,4141049,4141183,4141239,4141393,4141411,4141468,4141581,4141697,4141725,4141733,4142018,4142233,4142420,4142503,4142555,4142607,4142610,4142657,4142673,4142729,4143132,4143245,4143318,6512436,10200083,10200095,10202524,10202535,10202562,16200026,16200132,19000288,19000427,19000616,19001995,19001999,19002244,19002249,19002252,19002257,19002548,19002671,19002880,19003319,19003321,19003323,19003407,19003408,19003409,19003703,19003994,19004037,19004038,19004165,19004183,19004185,19004187,19004199,19004201,41317155',authuser:0,kscs:'c9c918f0_p94UWrLhB4zWjwTfgp0o',u:'c9c918f0',kGL:'CA'};google.kHL='en-CA';})();(function(){google.lc=[];google.li=0;google.getEI=function(a){for(var b;a&&(!a.g"
                  "etAttribute||!(b=a.getAttribute(\"eid\")));)a=a.parentNode;return b||google.kEI};google.getLEI=function(a){for(var b=null;a&&(!a.getAttribute||!(b=a.getAttribute(\"leid\")));)a=a.parentNode;return b};google.https=function(){return\"https:\"==window.location.protocol};google.ml=function(){return null};google.wl=function(a,b){try{google.ml(Error(a),!1,b)}catch(d){}};google.time=function(){return(new Date).getTime()};google.log=function(a,b,d,c,g){if(a=google.logUrl(a,b,d,c,g)){b=new Image;var e=google.lc,f=google.li;e[f]=b;b.onerror=b.onload=b.onabort=function(){delete e[f]};google.vel&&google.vel.lu&&google.vel.lu(a);b.src=a;google.li=f+1}};google.logUrl=function(a,b,d,c,g){var e=\"\",f=google.ls||\"\";d||-1!=b.search(\"&ei=\")||(e=\"&ei=\"+google.getEI(c),-1==b.search(\"&lei=\")&&(c=google.getLEI(c))&&(e+=\"&lei=\"+c));c=\"\";!d&&google.cshid&&-1==b.search(\"&cshid=\")&&(c=\"&cshid=\"+google.cshid);a=d||\"/\"+(g||\"gen_204\")+\"?atyp=i&ct=\"+a+\"&cad=\"+b+e+f+\"&zx=\"+google.time()+c;/^http:/i.test(a)&&google.https()&&(google.ml(Error(\"a\"),!1"
                  ",{src:a,glmm:1}),a=\"\");return a};}).call(this);(function(){google.y={};google.x=function(a,b){if(a)var c=a.id;else{do c=Math.random();while(google.y[c])}google.y[c]=[a,b];return!1};google.lm=[];google.plm=function(a){google.lm.push.apply(google.lm,a)};google.lq=[];google.load=function(a,b,c){google.lq.push([[a],b,c])};google.loadAll=function(a,b){google.lq.push([a,b])};}).call(this);google.f={};var a=window.location,b=a.href.indexOf(\"#\");if(0<=b){var c=a.href.substring(b+1);/(^|&)q=/.test(c)&&-1==c.indexOf(\"#\")&&a.replace(\"/search?\"+c.replace(/(^|&)fp=[^&]*/g,\"\")+\"&cad=h\")};</script><style>#gbar,#guser{font-size:13px;padding-top:1px !important;}#gbar{height:22px}#guser{padding-bottom:7px !important;text-align:right}.gbh,.gbd{border-top:1px solid #c9d7f1;font-size:1px}.gbh{height:0;position:absolute;top:24px;width:100%}@media all{.gb1{height:22px;margin-right:.5em;vertical-align:top}#gbar{float:left}}a.gb1,a.gb4{text-decoration:underline !important}a.gb1,a.gb4{color:#00c !important}.gbi .gb4{color:#dd8e27 !imp"
                  "ortant}.gbf .gb4{color:#900 !important}\012</style><style>body,td,a,p,.h{font-family:arial,sans-serif}body{margin:0;overflow-y:scroll}#gog{padding:3px 8px 0}td{line-height:.8em}.gac_m td{line-height:17px}form{margin-bottom:20px}.h{color:#36c}.q{color:#00c}.ts td{padding:0}.ts{border-collapse:collapse}em{font-weight:bold;font-style:normal}.lst{height:25px;width:496px}.gsfi,.lst{font:18px arial,sans-serif}.gsfs{font:17px arial,sans-serif}.ds{display:inline-box;display:inline-block;margin:3px 0 4px;margin-left:4px}input{font-family:inherit}a.gb1,a.gb2,a.gb3,a.gb4{color:#11c !important}body{background:#fff;color:black}a{color:#11c;text-decoration:none}a:hover,a:active{text-decoration:underline}.fl a{color:#36c}a:visited{color:#551a8b}a.gb1,a.gb4{text-decoration:underline}a.gb3:hover{text-decoration:none}#ghead a.gb2:hover{color:#fff !important}.sblc{padding-top:5px}.sblc a{display:block;margin:2px 0;margin-left:13px;font-size:11px}.lsbb{background:#eee;border:solid 1px;border-color:#ccc #999 #999 #ccc;height:30px}.l"
                  "sbb{display:block}.ftl,#fll a{display:inline-block;margin:0 12px}.lsb{background:url(/images/nav_logo229.png) 0 -261px repeat-x;border:none;color:#000;cursor:pointer;height:30px;margin:0;outline:0;font:15px arial,sans-serif;vertical-align:top}.lsb:active{background:#ccc}.lst:focus{outline:none}</style><script></script><link href=\"/images/branding/product/ico/googleg_lodp.ico\" rel=\"shortcut icon\"></head><body bgcolor=\"#fff\"><script>(function(){var src='/images/nav_logo229.png';var iesg=false;document.body.onload = function(){window.n && window.n();if (document.images){new Image().src=src;}\012if (!iesg){document.f&&document.f.q.focus();document.gbqf&&document.gbqf.q.focus();}\012}\012})();</script><div id=\"mngb\"> <div id=gbar><nobr><b class=gb1>Search</b> <a class=gb1 href=\"http://www.google.ca/imghp?hl=en&tab=wi\">Images</a> <a class=gb1 href=\"http://maps.google.ca/maps?hl=en&tab=wl\">Maps</a> <a class=gb1 href=\"https://play.google.com/?hl=en&tab=w8\">Play</a> <a class=gb1 href=\"http://www.youtube.com/?gl=CA&tab=w1\">YouT"
                  "ube</a> <a class=gb1 href=\"http://news.google.ca/nwshp?hl=en&tab=wn\">News</a> <a class=gb1 href=\"https://mail.google.com/mail/?tab=wm\">Gmail</a> <a class=gb1 href=\"https://drive.google.com/?tab=wo\">Drive</a> <a class=gb1 style=\"text-decoration:none\" href=\"https://www.google.ca/intl/en/options/\"><u>More</u> &raquo;</a></nobr></div><div id=guser width=100%><nobr><span id=gbn class=gbi></span><span id=gbf class=gbf></span><span id=gbe></span><a href=\"http://www.google.ca/history/optout?hl=en\" class=gb4>Web History</a> | <a  href=\"/preferences?hl=en\" class=gb4>Settings</a> | <a target=_top id=gb_70 href=\"https://accounts.google.com/ServiceLogin?hl=en&passive=true&continue=http://www.google.ca/%3Fgfe_rd%3Dcr%26amp%253Bdcr%3D0%26amp%253Bei%3Di94UWuXoBKLd8gf8vJ7ADw\" class=gb4>Sign in</a></nobr></div><div class=gbh style=left:0></div><div class=gbh style=right:0></div> </div><center><br clear=\"all\" id=\"lgpd\"><div id=\"lga\"><div style=\"padding:28px 0 3px\"><div style=\"height:110px;width:276px;background:url(/images/bran"
                  "ding/googlelogo/1x/googlelogo_white_background_color_272x92dp.png) no-repeat\" title=\"Google\" align=\"left\" id=\"hplogo\" onload=\"window.lol&&lol()\"><div style=\"color:#777;font-size:16px;font-weight:bold;position:relative;top:70px;left:218px\" nowrap=\"\">Canada</div></div></div><br></div><form action=\"/search\" name=\"f\"><table cellpadding=\"0\" cellspacing=\"0\"><tr valign=\"top\"><td width=\"25%\">&nbsp;</td><td align=\"center\" nowrap=\"\"><input name=\"ie\" value=\"ISO-8859-1\" type=\"hidden\"><input value=\"en-CA\" name=\"hl\" type=\"hidden\"><input name=\"source\" type=\"hidden\" value=\"hp\"><input name=\"biw\" type=\"hidden\"><input name=\"bih\" type=\"hidden\"><div class=\"ds\" style=\"height:32px;margin:4px 0\"><input style=\"color:#000;margin:0;padding:5px 8px 0 6px;vertical-align:top\" autocomplete=\"off\" class=\"lst\" value=\"\" title=\"Google Search\" maxlength=\"2048\" name=\"q\" size=\"57\"></div><br style=\"line-height:0\"><span class=\"ds\"><span class=\"lsbb\"><input class=\"lsb\" value=\"Google Search\" name=\"btnG\" type=\"submit\"></span></span><span class=\"ds\"><sp"
                  "an class=\"lsbb\"><input class=\"lsb\" value=\"I'm Feeling Lucky\" name=\"btnI\" onclick=\"if(this.form.q.value)this.checked=1; else top.location='/doodles/'\" type=\"submit\"></span></span></td><td class=\"fl sblc\" align=\"left\" nowrap=\"\" width=\"25%\"><a href=\"/advanced_search?hl=en-CA&amp;authuser=0\">Advanced search</a><a href=\"/language_tools?hl=en-CA&amp;authuser=0\">Language tools</a></td></tr></table><input id=\"gbv\" name=\"gbv\" type=\"hidden\" value=\"1\"></form><div id=\"gac_scont\"></div><div style=\"font-size:83%;min-height:3.5em\"><br><div id=\"als\"><style>#als{font-size:small;margin-bottom:24px}#_eEe{display:inline-block;line-height:28px;}#_eEe a{padding:0 3px;}._lEe{display:inline-block;margin:0 2px;white-space:nowrap}._PEe{display:inline-block;margin:0 2px}</style><div id=\"_eEe\">Google offered in: <a href=\"http://www.google.ca/setprefs?sig=0_Nrexmzk2uFP6HD_LLgmWdbS6IPk%3D&amp;hl=fr&amp;source=homepage&amp;sa=X&amp;ved=0ahUKEwjy0KaTj9HXAhUM64MKHV9BBwUQ2ZgBCAU\">Fran\347ais</a> </div></div></div><span id=\"footer\"><div style=\"fo"
                  "nt-size:10pt\"><div style=\"margin:19px auto;text-align:center\" id=\"fll\"><a href=\"/intl/en/ads/\">Advertising\240Programs</a><a href=\"/services/\">Business Solutions</a><a href=\"https://plus.google.com/108349337900676782287\" rel=\"publisher\">+Google</a><a href=\"/intl/en/about.html\">About Google</a><a href=\"http://www.google.ca/setprefdomain?prefdom=US&amp;sig=__2h7-TyeEA-Ycvuy-h7wm1VloiTA%3D\" id=\"fehl\">Google.com</a></div></div><p style=\"color:#767676;font-size:8pt\">&copy; 2017 - <a href=\"/intl/en/policies/privacy/\">Privacy</a> - <a href=\"/intl/en/policies/terms/\">Terms</a></p></span></center><script>(function(){window.google.cdo={height:0,width:0};(function(){var a=window.innerWidth,b=window.innerHeight;if(!a||!b){var c=window.document,d=\"CSS1Compat\"==c.compatMode?c.documentElement:c.body;a=d.clientWidth;b=d.clientHeight}a&&b&&(a!=google.cdo.width||b!=google.cdo.height)&&google.log(\"\",\"\",\"/client_204?&atyp=i&biw=\"+a+\"&bih=\"+b+\"&ei=\"+google.kEI);}).call(this);})();</script><div id=\"xjsd\"></div><div id=\"xjsi\"><script>"
                  "(function(){function c(b){window.setTimeout(function(){var a=document.createElement(\"script\");a.src=b;google.timers&&google.timers.load.t&&google.tick(\"load\",{gen204:\"xjsls\",clearcut:31});document.getElementById(\"xjsd\").appendChild(a)},0)}google.dljp=function(b,a){google.xjsu=b;c(a)};google.dlj=c;}).call(this);(function(){var r=[];google.plm(r);})();if(!google.xjs){window._=window._||{};window._DumpException=window._._DumpException=function(e){throw e};google.dljp('/xjs/_/js/k\\x3dxjs.hp.en_US._et4UJgHZ58.O/m\\x3dsb_he,d/am\\x3dADA/rt\\x3dj/d\\x3d1/t\\x3dzcms/rs\\x3dACT90oH9Ja6joK1Np9LEj3lF11ThlBjLtA','/xjs/_/js/k\\x3dxjs.hp.en_US._et4UJgHZ58.O/m\\x3dsb_he,d/am\\x3dADA/rt\\x3dj/d\\x3d1/t\\x3dzcms/rs\\x3dACT90oH9Ja6joK1Np9LEj3lF11ThlBjLtA');google.xjs=1;}google.pmc={\"sb_he\":{\"agen\":true,\"cgen\":true,\"client\":\"heirloom-hp\",\"dh\":true,\"dhqt\":true,\"ds\":\"\",\"fl\":true,\"host\":\"google.ca\",\"isbh\":28,\"jam\":0,\"jsonp\":true,\"msgs\":{\"cibl\":\"Clear Search\",\"dym\":\"Did you mean:\",\"lcky\":\"I\\u0026#39;m Feeling Lucky\",\"lml\":\"Learn more\",\"oskt\":\"I";
    }

    //write pattern to file
    size_t remainingSize = size;
    while(remainingSize >= pattern.size())
    {
      fwrite(pattern.c_str(), 1, pattern.size(), f);
      remainingSize -= pattern.size();
    }
    if (remainingSize > 0)
    {
      fwrite(pattern.c_str(), 1, remainingSize, f);
    }
  }

  fclose(f);

  return EXIT_NO_ERROR;
}
