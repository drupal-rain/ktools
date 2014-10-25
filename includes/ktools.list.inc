<?php

class KtoolsListOptions {

  public static function filterFormats($status = -1, $account = NULL) {
    $options = array();
    $formats = filter_formats($account);
    foreach ($formats as $format) {
      if ($status != -1 && $format->status !== $status) {
        continue;
      }
      $options[$format->format] = $format->name;
    }

    return $options;
  }

}