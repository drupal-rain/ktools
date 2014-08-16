<?php

class URL {
  protected $url;
  protected $data = array(
    'scheme' => NULL,
    'host' => NULL,
    'port' => NULL,
    'user' => NULL,
    'pass' => NULL,
    'path' => NULL,
    'query' => NULL,
    'fragment' => NULL,
  );

  public function __construct($url) {
    $this->url = $url;
    $this->data = array_merge($this->data, parse_url($url));
  }

  public function __get($name) {
    return isset($this->data[$name]) ? $this->data[$name] : NULL;
  }

  public function __set($name, $value) {
    $this->data[$name] = $value;
  }

  public function getURL() {
    return $this->url;
  }

  public function setURL($url) {
    $this->url = $url;
    $this->data = array_merge($this->data, parse_url($url));
  }

  public function isSubPath(URL $url_parent) {
    if (strpos($this->path, $url_parent->path) !== FALSE) {
      return TRUE;
    }

    return FALSE;
  }
}

