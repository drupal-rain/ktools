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
   * @return mixed|boolean
   */
  public static function loadByURI($uri) {
    $fid = db_query('SELECT fid FROM {file_managed} WHERE uri = :uri', array(':uri' => $uri))->fetchField();
    if (!empty($fid)) {
      $file_object = file_load($fid);
      return $file_object;
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
