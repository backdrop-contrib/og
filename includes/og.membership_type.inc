<?php

/**
 * @file
 * A class used for group membership types.
 */
class OgMembershipType extends Entity {

  /**
   * Primary Key: Numeric group membership type ID.
   * 
   * @var integer
   */
  public $id;

  /**
   * The unified identifier for a group membership type.
   * 
   * @var $string
   */
  public $name;

  /**
   * Description for this group membership type.
   * 
   * @var string
   */
  public $description = '';

  /**
   * The name of the providing module if the entity has been defined in code.
   * 
   * @var string
   */
  public $module;

  /**
   * The language of this membership type.
   * 
   * @var string
   */
  public $language;

  /**
   * The exportable status of the entity.
   * 
   * @var bool
   */
  public $status;

  public function id() {
    return isset($this->id) ? $this->id : NULL;
  }

  /**
   * Implements EntityInterface::entityType().
   */
  public function entityType() {
    return 'og_membership_type';
  }

  /**
   * Implements EntityInterface::label().
   */
  public function label() {
    return isset($this->description) ? $this->description : NULL;
  }

  /**
   * Implements EntityInterface::uri().
   */
  public function uri() {
    return array(
      'path' => 'admin/config/group/group-membership/manage/' . $this->name,
      'options' => array(),
    );
  }

  public function __construct($values = array()) {
    parent::__construct($values, 'og_membership_type');
  }
}
