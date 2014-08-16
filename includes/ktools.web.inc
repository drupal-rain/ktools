<?php

class KtoolsURL {

  public static function urlScheme($url) {
    return parse_url($url, PHP_URL_SCHEME);
  }

}

