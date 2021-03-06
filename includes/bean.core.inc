<?php

/**
 * The Bean entity class
 */
class Bean extends Entity {
  public $label;
  public $title;
  public $type;
  public $data;
  protected $plugin;

  /**
   * Get Plugin info
   */
  public function getInfo($key = NULL) {
    return $this->plugin->getInfo($key);
  }

  public function defaultLabel() {
    return $this->label;
  }

  public function __construct($values = array()) {
    parent::__construct($values, 'bean');

    // load the plugin info
    $this->plugin = bean_load_plugin_class($this->type);
    $this->setFields();
  }

  /**
   * Set the fields from the defaults and plugin
   */
  protected function setFields() {
    $values = unserialize($this->data);
    foreach ($this->plugin->values() as $field => $default) {
      $this->$field = isset($values[$field]) ? $values[$field] : $default;
    }
  }

  /**
   * Override this in order to implement a custom default URI and specify
   * 'entity_class_uri' as 'uri callback' hook_entity_info().
   */
  protected function defaultUri() {
    return array('path' => 'block/' . $this->identifier());
  }

  /**
   * Get the plugin form
   */
  public function getForm() {
    return $this->plugin->form($this);
  }

  /**
   * Validate the plugin form
   */
  public function validate($values) {
    $this->plugin->validate($values);
  }

  /**
   * Edit URL
   */
  public function editURL() {
    return 'block/' . $this->identifier() . '/edit';
  }

  /**
   * Set the values from a plugin
   */
  public function setValues($values) {
    $this->data = array();
    foreach ($this->plugin->values() as $field => $value) {
      $this->data[$field] = $values[$field];
    }
  }

  /**
   * Generate an array for rendering the entity.
   *
   * @see entity_view()
   */
  public function view($view_mode = 'full', $langcode = NULL) {
    $content = parent::view();

    return $this->plugin->view($this, $content, $view_mode, $langcode);
  }
}