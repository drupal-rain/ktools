<?php

/**
 * @file
 * String related helper.
 */

define('FILE_OP_COPY', 1);
define('FILE_OP_MOVE', -1);

class KtoolsFilePath {

  public static function getDirname($path) {
    return pathinfo(drupal_realpath($path), PATHINFO_DIRNAME);
  }

  public static function getBasename($path) {
    return pathinfo(drupal_realpath($path), PATHINFO_BASENAME);
  }

  public static function getExtension($path) {
    return pathinfo(drupal_realpath($path), PATHINFO_EXTENSION);
  }

  public static function getFilename($path) {
    return pathinfo(drupal_realpath($path), PATHINFO_FILENAME);
  }

}

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
   *
   * @see file_unmanaged_delete_recursive().
   * @link https://api.drupal.org/api/drupal/includes%21file.inc/function/file_unmanaged_delete_recursive/7
   *
   * file_unmanaged_delete_recursive() is said to not delete symbolic links properly?
   */
  public static function deleteDir($dirname) {
    $dirname = drupal_realpath($dirname);
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
          self::deleteDir($dirname . '/' . $file);
        }
      }
    }
    closedir($dir_handle);
    rmdir($dirname);

    return TRUE;
  }

  /**
   * Scan directory and save files inside to drupal.
   *
   * @todo Need improvement.
   * @issue Need to avoid important dir call like stream wrapper root.
   * @todo File Status, Replace settings.
   */
  public static function scanSave($dir, $move = TRUE, $dir_dest = NULL, $mask = '/[.]*/', $options = array()) {
    $files = file_scan_directory($dir, $mask, $options);
    // If it's unknown to drupal, copy to temporary:// first.
    if (!file_valid_uri($dir)) {
      foreach ($files as $file) {
        $dir_dest_tmp = 'temporary://' . self::getFolder($dir);
        dsm(drupal_realpath($dir_dest_tmp));
        file_prepare_directory($dir_dest_tmp, FILE_CREATE_DIRECTORY);
        $file->uri = file_unmanaged_copy($file->uri, $dir_dest_tmp . DIRECTORY_SEPARATOR . $file->filename);
        // Force move in this situation.
        $move = TRUE;
        $dir_dest = file_default_scheme() . '://' . self::getFolder($dir);
        // Flag Remove
        $remove = TRUE;
        $dir_remove = $dir_dest_tmp;
      }
    }
    $files_obj = array();
    foreach ($files as $file) {
      // Try to load managed file.
      if (!$file_obj = KtoolsFile::loadByURI($file->uri)) {
        $file_obj = file_save($file);
      }
      if ($move) {
        if ($dir_dest) {
          $dest = $dir_dest . DIRECTORY_SEPARATOR . $file_obj->filename;
          file_prepare_directory($dir_dest, FILE_CREATE_DIRECTORY);
        }
        else {
          $dest = file_default_scheme() . '://' . $file_obj->filename;
        }
        // Replace or Rename
        $replace = isset($remove) ? FILE_EXISTS_REPLACE : FILE_EXISTS_RENAME;
        $files_obj[] = file_move($file_obj, $dest, $replace);
        //
        $remove = TRUE;
        $dir_remove = $dir;
      }
      else {
        $files_obj[] = $file_obj;
      }
    }

    if (isset($remove)) {
      self::deleteDir($dir_remove);
    }

    return $files_obj;
  }

  /**
   * Get the last directory from the path.
   *
   * @issue If root?
   */
  public static function getFolder($uri) {
    $path = drupal_realpath($uri);
    $pieces = explode(DIRECTORY_SEPARATOR, $path);

    return end($pieces);
  }

}

class KtoolsArchive {

  /**
   * Compress array of file objects into archive.
   */
  public static function compressZip($files) {

  }

  /**
   * Extract archive and save as managed files.
   */
  public static function extractZip($archive, $path) {
    $archiver = archiver_get_archiver($archive);
    $archiver->extract($path);
  }

}
