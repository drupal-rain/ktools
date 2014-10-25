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

  /**
   * Check if string starts with $needle.
   *
   * @param $haystack
   * @param $needle
   * @return bool
   */
  public static function startsWith($haystack, $needle) {
    return $needle === "" || strpos($haystack, $needle) === 0;
  }

  /**
   * Check if string ends with $needle.
   *
   * @param $haystack
   * @param $needle
   * @return bool
   */
  public static function endsWith($haystack, $needle) {
    return $needle === "" || substr($haystack, -strlen($needle)) === $needle;
  }

}

class KtoolsArray {

  public static function defaultValue($arr, $index, $default_value) {
    $var =&$arr;
    if (is_array($index)) {
      foreach ($index as $child) {
        $var = &$var[$child];
      }
    }
    else {
      $var = &$var[$index];
    }

    return isset($var) ? $var : $default_value;
  }

}

class KtoolsObject {

  public static function defaultValue($obj, $property, $default_value) {
    $var =&$obj;
    if (is_array($obj->{$property})) {
      foreach ($property as $child) {
        $var = &$var->{$child};
      }
    }
    else {
      $var = &$var->{$property};
    }

    return isset($var) ? $var : $default_value;
  }

}
