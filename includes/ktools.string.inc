<?php

/**
 * @file
 * String related helper.
 */

class KtoolsString {

  /**
   * Generate a random string used chars provided by source.
   *
   * @param string $src
   *   Source characters.
   * @param int $len
   *   Length of the return random string.
   * @return string
   */
  public static function strRandom($src, $len) {
    $ret = '';
    $rand_max = strlen($src);
    for ($i = 0; $i < $len; $i++) {
      $ret .= $src[mt_rand(0, $rand_max)];
    }

    return $ret;
  }
  
  /**
   * More options to compare a string.
   *
   * @param string $value
   * @param NULL|string|array $compare_to
   */
  public static function strCompare($value, $compare_to) {
    $ret = TRUE;
    if (is_array($compare_to)) {
      if (!in_array($value, $compare_to)) {
        $ret = FALSE;
      }
    }
    else {
      if ($value != $compare_to) {
        $ret = FALSE;
      }
    }

    return $ret;
  }

}
