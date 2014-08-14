<?php

/**
 * @file
 * String related helper.
 */

class KtoolsFile {

  /**
   * Helper function to load file entity by uri.
   *
   * @param string $uri
   * @return boolean|stdClass
   */
  public static function loadByURI($uri) {
    $fid = db_query('SELECT fid FROM {file_managed} WHERE uri = :uri', array(':uri' => $uri))->fetchField();
    if (!empty($fid)) {
      $file = file_load($fid);
      return $file;
    }
    return FALSE;
  }

  /**
   * Helper function to load file entity by relative path.
   */
  public static function loadByPath($path, $scheme = NULL) {
    $stream_wrapper = $scheme ? $scheme : file_default_scheme();
    $uri = $stream_wrapper . '://' . $path;
    $uri = file_stream_wrapper_uri_normalize($uri);

    return self::loadByURI($uri);
  }

  /**
   * Download and save a web file.
   */
  public static function wgetSave($url, $dest = NULL) {
    if (
      valid_url($url)
      && $data = file_get_contents($url)
    ) {
      if ($dest === NULL) {
        $dest = 'public://' . basename($url);
      }
      if ($file = file_save_data($data, $dest)) {
        return $file;
      }
    }

    return FALSE;
  }

}

class KtoolsDir {

  /**
   * Delete non-empty directory by recursively delete sub files and
   * sub directories.
   */
  public static function delete($dirname) {
    if (is_dir($dirname)) {
      $dir_handle = opendir($dirname);
    }
    if (!$dir_handle) {
      return FALSE;
    }
    while($file = readdir($dir_handle)) {
      if ($file != '.' && $file != '..') {
        if (!is_dir($dirname . '/' . $file)) {
          unlink($dirname . '/' . $file);
        }
        else {
          self::delete($dirname . '/' . $file);
        }
      }
    }
    closedir($dir_handle);
    rmdir($dirname);

    return TRUE;
  }

}

class KtoolsArchive {

  public static function compress() {

  }

  public static function extract() {

  }

}
