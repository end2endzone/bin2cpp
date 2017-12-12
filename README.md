![bin2cpp logo](http://www.end2endzone.com/wp-content/uploads/2015/01/bin2cpp-featured-image.png)

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Github Releases](https://img.shields.io/github/release/end2endzone/bin2cpp.svg)](https://github.com/end2endzone/bin2cpp/releases)
[![Build status](https://ci.appveyor.com/api/projects/status/q9k0ebb971o475qi/branch/master?svg=true)](https://ci.appveyor.com/project/end2endzone/bin2cpp/branch/master)
[![Tests status](https://img.shields.io/appveyor/tests/end2endzone/bin2cpp/master.svg)](https://ci.appveyor.com/project/end2endzone/bin2cpp/branch/master/tests)

AppVeyor build statistics:

[![Build statistics](https://buildstats.info/appveyor/chart/end2endzone/bin2cpp)](https://ci.appveyor.com/project/end2endzone/bin2cpp/branch/master)



# bin2cpp

bin2cpp is a command line tool for embedding small files (like images, icons or raw data files) into a C++ executable.

It's main features are:

*   Easily converts small files as C++ source code for embedding into a C++ executable.
*   Content extraction supported by a unique function call for each embedded file.
*   Supports a multiple embedded files at once.
*   Makes it harder for resource hacker to modify or steal the embedded files.
*   No third party libraries required for retrieving the data of the embedded files.

bin2cpp is not implemented using [executable resources](http://en.wikipedia.org/wiki/Resource_(Windows)). Instead, bin2cpp creates multiple function calls for retrieving the data which makes it harder to steal the executable's content. It also makes it even harder to replace an existing resource of the executable.

It is designed to be easy to use by developers and to provide easy call functions to get access to the data of the embedded files.

The generated functions that reads and extracts the embedded content does not rely on external libraries so you don't need to setup your projects to use any third party library to start using bin2cpp. All your embedded data can be accessed right away.

# Usage

**Usage:** bin2cpp [input file] [output folder] [header filename] [function identifier] [chunk size] [-override].

| Argument            | Description                                                                                    |
|---------------------|------------------------------------------------------------------------------------------------|
| input file          | Path to the target file to embed in a C++ source code.                                         |
| output folder       | Output path to create generated source code. ie: .\generated_files\                            |
| header filename     | File name of the C++ Header file. ie: SplashScreen.h                                           |
| function identifier | Identifier of the function name that is used to get an instance of the file. ie: SplashScreen  |
| chunk size          | Size of each string segments. Defaults to 200]                                                 |
| override            | Tells bin2cpp to over write the destination files.                                             |


# Example

### Input File:
(index page of https://choosealicense.com/)
```html
<!DOCTYPE html>
<html lang="en">
  <head itemscope itemtype="http://schema.org/WebSite">
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <link rel="stylesheet" href="//fonts.googleapis.com/css?family=Chivo:900">
    <link rel="stylesheet" href="/assets/css/application.css?v=7213ccf8ac07263d66dd6aa8bf955d1bbec06117">
    <link rel="shortcut icon" href="/favicon.ico" type="image/x-icon">
    <!--[if lt IE 9]>
    <script src="/assets/vendor/html5shiv/dist/html5shiv.js"></script>
    <script src="/assets/vendor/selectivizr/selectivizr.js"></script>
    <![endif]-->
    <meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1, user-scalable=no">
    <!-- Begin Jekyll SEO tag v2.3.0 -->
<title>Choose an open source license | Choose a License</title>
<meta property="og:title" content="Choose an open source license" />
<meta property="og:locale" content="en_US" />
<meta name="description" content="Non-judgmental guidance on choosing a license for your open source project" />
<meta property="og:description" content="Non-judgmental guidance on choosing a license for your open source project" />
<link rel="canonical" href="https://choosealicense.com/" />
<meta property="og:url" content="https://choosealicense.com/" />
<meta property="og:site_name" content="Choose a License" />
<meta name="twitter:card" content="summary" />
<meta name="twitter:site" content="@github" />
<script type="application/ld+json">
{"name":"Choose a License","description":"Non-judgmental guidance on choosing a license for your open source project","author":null,"@type":"WebSite","url":"https://choosealicense.com/","image":null,"publisher":null,"headline":"Choose an open source license","dateModified":null,"datePublished":null,"sameAs":null,"mainEntityOfPage":null,"@context":"http://schema.org"}</script>
<!-- End Jekyll SEO tag -->
  </head>
  <body class="default home">
    <div class="container">
        <h1>Choose an open source license</h1>
<h2>
  <span>{</span>
  Which of the following best describes your situation?
  <span>}</span>
</h2>
<ul class="triptych situations clearfix">
  <li class="whatever">
    <a href="licenses/mit">
      <span class="triptych-sprite three-arrows"></span>
      <h3>I want it simple and permissive.</h3>
    </a>
    <p>The <a href="licenses/mit">MIT License</a> is a permissive license that is short and to the point. It lets people do anything they want with your code as long as they provide attribution back to you and don’t hold you liable.</p>
    <p>
        <a href="https://github.com/babel/babel/blob/master/LICENSE">Babel</a>,
        <a href="https://github.com/dotnet/corefx/blob/master/LICENSE.TXT">.NET Core</a>, and
        <a href="https://github.com/rails/rails/blob/master/MIT-LICENSE">Rails</a>
    use the MIT License.
    </p>
  </li>
  <li class="patents">
    <a href="licenses/apache-2.0/">
      <span class="triptych-sprite lightbulb"></span>
      <h3>I’m concerned about patents.</h3>
    </a>
    <p>The <a href="licenses/apache-2.0/">Apache License 2.0</a> is a permissive license similar to the MIT License, but also provides an express grant of patent rights from contributors to users.</p>
    <p>
        <a href="https://github.com/elastic/elasticsearch/blob/master/LICENSE.txt">Elasticsearch</a>,
        <a href="https://github.com/kubernetes/kubernetes/blob/master/LICENSE">Kubernetes</a>, and
        <a href="https://github.com/apple/swift/blob/master/LICENSE.txt">Swift</a>
    use the Apache License 2.0.
    </p>
  </li>
  <li class="copyleft">
    <a href="licenses/gpl-3.0/">
      <span class="triptych-sprite circular"></span>
      <h3>I care about sharing improvements.</h3>
    </a>
    <p>The <a href="licenses/gpl-3.0/">GNU GPLv3</a> is a copyleft license that requires anyone who distributes your code or a derivative work to make the source available under the same terms, and also provides an express grant of patent rights from contributors to users.</p>
    <p>
        <a href="https://github.com/ansible/ansible/blob/devel/COPYING">Ansible</a>,
        <a href="https://git.savannah.gnu.org/cgit/bash.git/tree/COPYING">Bash</a>, and
        <a href="https://git.gnome.org/browse/gimp/tree/COPYING">GIMP</a>
    use the GNU GPLv3.
    </p>
  </li>
</ul>
<h2>
  <span>{</span>
  What if none of these work for me?
  <span>}</span>
</h2>
<ul class="triptych see-more clearfix">
  <li>
    <h3>My project isn’t software.</h3>
    <p>
      <a href="non-software">There are licenses for that</a>.
    </p>
  </li>
  <li>
    <h3>I want more choices.</h3>
    <p><a href="licenses">More licenses are available</a>.</p>
  </li>
  <li>
    <h3>I don’t want to choose a license.</h3>
    <p><a href="no-license">You don’t have to</a>.</p>
  </li>
</ul>
      <footer class="site-footer clearfix">
        <nav>
          <a href="/about/">About</a>
          <a href="/terms-of-service/">Terms of Service</a>
        </nav>
        <p>
          The content of this site is licensed under the <a href="https://creativecommons.org/licenses/by/3.0/">
          Creative Commons Attribution 3.0 Unported License</a>.
        </p>
        <div class="with-love">
          Curated with &lt;3 by <a href="https://github.com">GitHub, Inc.</a> and <a href="https://github.com/github/choosealicense.com">You!</a>
        </div>
      </footer>
    </div> <!-- /container -->
    <script>
      !function(L,I,C,e,N,S,E){L.GoogleAnalyticsObject=N,L[N]=L[N]||function(){
      (L[N].q=L[N].q||[]).push(arguments)},L[N].l=+new Date,S=I.createElement(C),
      E=I.getElementsByTagName(C)[0],S.src=e,E.parentNode.insertBefore(S,E)
      }(window,document,"script","//www.google-analytics.com/analytics.js","ga");
      ga('create', 'UA-3769691-24', 'choosealicense.com');
      ga('send', 'pageview');
    </script>
  </body>
</html>
```

### Command:
```batchfile
bin2cpp.exe choosealicense.html .\outdir resourcechoosealicense.h ChooseALicence 250 -override
```

### Console output

```batchfile
bin2cpp v1.4
Copyright (C) 2013-2017 end2endzone.com. All rights reserved.
Embedding "choosealicense.html" into "resourcechoosealicense.h" using chunks of 250 bytes overriding existing files...
Done.
```

### Will produce:

resourcechoosealicense.h

```cpp
/**
 * This file was generated by bin2cpp v1.4.
 * Copyright (C) 2013-2017 end2endzone.com. All rights reserved.
 * Do not modify this file.
 */
#pragma once
#include <stddef.h>
namespace bin2cpp
{
  #ifndef BIN2CPP_EMBEDEDFILE_CLASS
  #define BIN2CPP_EMBEDEDFILE_CLASS
  class File
  {
  public:
    virtual size_t getSize() const = 0;
    virtual size_t getSegmentSize() const = 0;
    virtual size_t getNumSegments() const = 0;
    virtual const char * getFilename() const = 0;
    virtual const char * getSegment(size_t iIndex, size_t & oLength) const = 0;
    virtual const char * getMd5() const = 0;
    virtual char * newBuffer() const = 0;
    virtual bool save(const char * iFilename) const = 0;
  };
  #endif
  const File & getChooseALicenceFile();
}; //bin2cpp
```

resourcechoosealicense.cpp

```cpp
/**
 * This file was generated by bin2cpp v1.4.
 * Copyright (C) 2013-2017 end2endzone.com. All rights reserved.
 * Do not modify this file.
 */
#include "resourcechoosealicense.h"
#include <stdio.h> //for FILE
#include <string> //for memcpy
namespace bin2cpp
{
  class ChooseALicenceFile : public virtual bin2cpp::File
  {
  public:
    ChooseALicenceFile() {}
    ~ChooseALicenceFile() {}
    virtual size_t getSize() const { return 6000; }
    virtual size_t getSegmentSize() const { return 250; }
    virtual size_t getNumSegments() const { return 24; }
    virtual const char * getFilename() const { return "choosealicense.html"; }
    virtual const char * getSegment(size_t iIndex, size_t & oLength) const
    {
      oLength = 0;
      if (iIndex >= getNumSegments())
        return NULL;
      const char * buffer = NULL;
      size_t index = 0;
      oLength = getSegmentSize();
      buffer = "<!DOCTYPE html>\r\n<html lang=\"en\">\r\n  <head itemscope itemtype=\"http://schema.org/WebSite\">\r\n    <meta charset=\"utf-8\">\r\n    <meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\r\n    <link rel=\"stylesheet\" href=\"//fonts.googleapis.com/css\?family=Chiv"; if (iIndex == index) return buffer; index++;
      buffer = "o:900\">\r\n    <link rel=\"stylesheet\" href=\"/assets/css/application.css\?v=7213ccf8ac07263d66dd6aa8bf955d1bbec06117\">\r\n    <link rel=\"shortcut icon\" href=\"/favicon.ico\" type=\"image/x-icon\">\r\n    <!--[if lt IE 9]>\r\n    <script src=\"/assets/vendor/html5sh"; if (iIndex == index) return buffer; index++;
      buffer = "iv/dist/html5shiv.js\"></script>\r\n    <script src=\"/assets/vendor/selectivizr/selectivizr.js\"></script>\r\n    <![endif]-->\r\n    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1, maximum-scale=1, user-scalable=no\">\r\n    <!-- Begin Jeky"; if (iIndex == index) return buffer; index++;
      buffer = "ll SEO tag v2.3.0 -->\r\n<title>Choose an open source license | Choose a License</title>\r\n<meta property=\"og:title\" content=\"Choose an open source license\" />\r\n<meta property=\"og:locale\" content=\"en_US\" />\r\n<meta name=\"description\" content=\"Non-judgmen"; if (iIndex == index) return buffer; index++;
      buffer = "tal guidance on choosing a license for your open source project\" />\r\n<meta property=\"og:description\" content=\"Non-judgmental guidance on choosing a license for your open source project\" />\r\n<link rel=\"canonical\" href=\"https://choosealicense.com/\" />\r"; if (iIndex == index) return buffer; index++;
      buffer = "\n<meta property=\"og:url\" content=\"https://choosealicense.com/\" />\r\n<meta property=\"og:site_name\" content=\"Choose a License\" />\r\n<meta name=\"twitter:card\" content=\"summary\" />\r\n<meta name=\"twitter:site\" content=\"@github\" />\r\n<script type=\"application/"; if (iIndex == index) return buffer; index++;
      buffer = "ld+json\">\r\n{\"name\":\"Choose a License\",\"description\":\"Non-judgmental guidance on choosing a license for your open source project\",\"author\":null,\"@type\":\"WebSite\",\"url\":\"https://choosealicense.com/\",\"image\":null,\"publisher\":null,\"headline\":\"Choose an o"; if (iIndex == index) return buffer; index++;
      buffer = "pen source license\",\"dateModified\":null,\"datePublished\":null,\"sameAs\":null,\"mainEntityOfPage\":null,\"@context\":\"http://schema.org\"}</script>\r\n<!-- End Jekyll SEO tag -->\r\n  </head>\r\n  <body class=\"default home\">\r\n    <div class=\"container\">\r\n        <"; if (iIndex == index) return buffer; index++;
      buffer = "h1>Choose an open source license</h1>\r\n<h2>\r\n  <span>{</span>\r\n  Which of the following best describes your situation\?\r\n  <span>}</span>\r\n</h2>\r\n<ul class=\"triptych situations clearfix\">\r\n  <li class=\"whatever\">\r\n    <a href=\"licenses/mit\">\r\n      <s"; if (iIndex == index) return buffer; index++;
      buffer = "pan class=\"triptych-sprite three-arrows\"></span>\r\n      <h3>I want it simple and permissive.</h3>\r\n    </a>\r\n    <p>The <a href=\"licenses/mit\">MIT License</a> is a permissive license that is short and to the point. It lets people do anything they wan"; if (iIndex == index) return buffer; index++;
      buffer = "t with your code as long as they provide attribution back to you and don\222t hold you liable.</p>\r\n    <p>\r\n        <a href=\"https://github.com/babel/babel/blob/master/LICENSE\">Babel</a>,\r\n        <a href=\"https://github.com/dotnet/corefx/blob/master/L"; if (iIndex == index) return buffer; index++;
      buffer = "ICENSE.TXT\">.NET Core</a>, and\r\n        <a href=\"https://github.com/rails/rails/blob/master/MIT-LICENSE\">Rails</a>\r\n    use the MIT License.\r\n    </p>\r\n  </li>\r\n  <li class=\"patents\">\r\n    <a href=\"licenses/apache-2.0/\">\r\n      <span class=\"triptych-"; if (iIndex == index) return buffer; index++;
      buffer = "sprite lightbulb\"></span>\r\n      <h3>I\222m concerned about patents.</h3>\r\n    </a>\r\n    <p>The <a href=\"licenses/apache-2.0/\">Apache License 2.0</a> is a permissive license similar to the MIT License, but also provides an express grant of patent rights"; if (iIndex == index) return buffer; index++;
      buffer = " from contributors to users.</p>\r\n    <p>\r\n        <a href=\"https://github.com/elastic/elasticsearch/blob/master/LICENSE.txt\">Elasticsearch</a>,\r\n        <a href=\"https://github.com/kubernetes/kubernetes/blob/master/LICENSE\">Kubernetes</a>, and\r\n    "; if (iIndex == index) return buffer; index++;
      buffer = "    <a href=\"https://github.com/apple/swift/blob/master/LICENSE.txt\">Swift</a>\r\n    use the Apache License 2.0.\r\n    </p>\r\n  </li>\r\n  <li class=\"copyleft\">\r\n    <a href=\"licenses/gpl-3.0/\">\r\n      <span class=\"triptych-sprite circular\"></span>\r\n     "; if (iIndex == index) return buffer; index++;
      buffer = " <h3>I care about sharing improvements.</h3>\r\n    </a>\r\n    <p>The <a href=\"licenses/gpl-3.0/\">GNU GPLv3</a> is a copyleft license that requires anyone who distributes your code or a derivative work to make the source available under the same terms, "; if (iIndex == index) return buffer; index++;
      buffer = "and also provides an express grant of patent rights from contributors to users.</p>\r\n    <p>\r\n        <a href=\"https://github.com/ansible/ansible/blob/devel/COPYING\">Ansible</a>,\r\n        <a href=\"https://git.savannah.gnu.org/cgit/bash.git/tree/COPYI"; if (iIndex == index) return buffer; index++;
      buffer = "NG\">Bash</a>, and\r\n        <a href=\"https://git.gnome.org/browse/gimp/tree/COPYING\">GIMP</a>\r\n    use the GNU GPLv3.\r\n    </p>\r\n  </li>\r\n</ul>\r\n<h2>\r\n  <span>{</span>\r\n  What if none of these work for me\?\r\n  <span>}</span>\r\n</h2>\r\n<ul class=\"triptych"; if (iIndex == index) return buffer; index++;
      buffer = " see-more clearfix\">\r\n  <li>\r\n    <h3>My project isn\222t software.</h3>\r\n    <p>\r\n      <a href=\"non-software\">There are licenses for that</a>.\r\n    </p>\r\n  </li>\r\n  <li>\r\n    <h3>I want more choices.</h3>\r\n    <p><a href=\"licenses\">More licenses are a"; if (iIndex == index) return buffer; index++;
      buffer = "vailable</a>.</p>\r\n  </li>\r\n  <li>\r\n    <h3>I don\222t want to choose a license.</h3>\r\n    <p><a href=\"no-license\">You don\222t have to</a>.</p>\r\n  </li>\r\n</ul>\r\n      <footer class=\"site-footer clearfix\">\r\n        <nav>\r\n          <a href=\"/about/\">About<"; if (iIndex == index) return buffer; index++;
      buffer = "/a>\r\n          <a href=\"/terms-of-service/\">Terms of Service</a>\r\n        </nav>\r\n        <p>\r\n          The content of this site is licensed under the <a href=\"https://creativecommons.org/licenses/by/3.0/\">\r\n          Creative Commons Attribution 3."; if (iIndex == index) return buffer; index++;
      buffer = "0 Unported License</a>.\r\n        </p>\r\n        <div class=\"with-love\">\r\n          Curated with &lt;3 by <a href=\"https://github.com\">GitHub, Inc.</a> and <a href=\"https://github.com/github/choosealicense.com\">You!</a>\r\n        </div>\r\n      </footer>"; if (iIndex == index) return buffer; index++;
      buffer = "\r\n    </div> <!-- /container -->\r\n    <script>\r\n      !function(L,I,C,e,N,S,E){L.GoogleAnalyticsObject=N,L[N]=L[N]||function(){\r\n      (L[N].q=L[N].q||[]).push(arguments)},L[N].l=+new Date,S=I.createElement(C),\r\n      E=I.getElementsByTagName(C)[0],S"; if (iIndex == index) return buffer; index++;
      buffer = ".src=e,E.parentNode.insertBefore(S,E)\r\n      }(window,document,\"script\",\"//www.google-analytics.com/analytics.js\",\"ga\");\r\n      ga(\'create\', \'UA-3769691-24\', \'choosealicense.com\');\r\n      ga(\'send\', \'pageview\');\r\n    </script>\r\n  </body>\r\n</html>\r\n\r\n"; if (iIndex == index) return buffer; index++;
      oLength = 0;
      oLength = 0;
      return NULL;
    }
    virtual const char * getMd5() const { return "bd3a043766eabe41c3b69bcc3f8f24db"; }
    virtual char * newBuffer() const
    {
      size_t size = getSize();
      char * buffer = new char[size];
      if (buffer == NULL)
        return NULL;
      size_t numSegments = getNumSegments();
      size_t segmentLength = 0;
      size_t index = 0;
      for(size_t i=0; i<numSegments; i++)
      {
        const char * segmentBuffer = getSegment(i, segmentLength);
        memcpy(&buffer[index], segmentBuffer, segmentLength);
        index += segmentLength;
      }
      return buffer;
    }
    virtual bool save(const char * iFilename) const
    {
      FILE * f = fopen(iFilename, "wb");
      if (!f) return false;
      size_t numSegments = getNumSegments();
      size_t segmentLength = 0;
      const char * buffer = NULL;
      for(size_t i=0; i<numSegments; i++)
      {
        buffer = getSegment(i, segmentLength);
        fwrite(buffer, 1, segmentLength, f);
      }
      fclose(f);
      return true;
    }
  };
  const File & getChooseALicenceFile() { static ChooseALicenceFile _instance; return _instance; }
}; //bin2cpp
```

# Screenshots

[![bin2cpp v1.3 Sample](http://www.end2endzone.com/wp-content/uploads/2015/01/bin2cpp-v1.3-done.png)](http://www.end2endzone.com/wp-content/uploads/2015/01/bin2cpp-v1.3-done.png)

bin2cpp v1.3 Sample

[![bin2cpp v1.3 Usage](http://www.end2endzone.com/wp-content/uploads/2015/01/bin2cpp-v1.3-usage.png)](http://www.end2endzone.com/wp-content/uploads/2015/01/bin2cpp-v1.3-usage.png) bin2cpp v1.3 Usage

# Compatible with

bin2cpp is only available for the Windows platform and has been tested with the following version of Windows:

*   Windows XP
*   Windows Vista
*   Windows 7

# License

MIT License

Copyright (c) 2017 end2endzone

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
