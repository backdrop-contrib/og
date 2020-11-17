<?php

/**
 * @file
 * Contains code related to the OG system of 'context'.
 *
 * Context, originally from Panels, is a method of packaging objects into
 * a more generic bundle and providing a plugin system so that a UI can
 * take advantage of them. The idea is that the context objects
 * represent 'the context' that a given operation (usually a page view)
 * is operating in or on.
 *
 * For example, when viewing a page, the 'context' is a node object. When
 * viewing a user, the 'context' is a user object. Contexts can also
 * have related contexts. For example, when viewing a 'node' you may need
 * to know something about the node author. Therefore, the node author
 * is a related context.
 */

/**
 * The context object is largely a wrapper around some other object, with
 * an interface to finding out what is contained and getting to both
 * the object and information about the object.
 *
 * Each context object has its own information, but some things are very
 * common, such as titles, data, keywords, etc. In particulare, the 'type'
 * of the context is important.
 */
class og_context {
  /**
   * @var string|array
   *   A string naming this specific context type. The values 'any' and 'none'
   *   are special:
   *    - 'any': used in is_type() to match any other type.
   *    - 'none': used to signal the type is not defined.
   */
  public $type;

  /**
   * @var mixed
   *   The data payload for this context object.
   */
  public $data;

  /**
   * @var string
   *   The title of this object.
   */
  public $title;

  /**
   * @var string
   *   The title of the page if this object exists
   */
  public $page_title;

  /**
   * @var string
   *   The identifier (in the UI) of this object.
   */
  public $identifier;

  /**
   * @var
   */
  public $argument;

  /**
   * @var string
   */
  public $keyword;

  /**
   * @var
   */
  public $original_argument;

  /**
   * @var array
   */
  public $restrictions;

  /**
   * @var bool
   */
  public $empty;

  /**
   * The og_context constructor.
   *
   * @param string $type
   *   The type name of this context. Should be unique. Use the machine_name
   *   conventions: lowercase, short, underscores and no spaces.
   * @param mixed $data
   *   The data payload, if required for this context.
   */
  public function __construct($type = 'none', $data = NULL) {
    $this->type = $type;
    $this->data = $data;
    $this->title = t('Unknown context');
    $this->page_title = '';
    $this->identifier = '';
    $this->keyword = '';
    $this->restrictions = array();
    $this->empty = FALSE;
    // Other vars are NULL.
  }

  /**
   * Determine whether this object is of type @var $type .
   *
   * Both the internal value ($this->type) and the supplied value ($type) can
   * be a string or an array of strings, and if one or both are arrays the match
   * succeeds if at least one common element is found.
   *
   * Type names
   *
   * @param string|array $type
   *   'type' can be:
   *    - 'any' to match all types (this is true of the internal value too).
   *    - an array of type name strings, when more than one type is acceptable.
   *
   * @return bool
   *   True if the type matches, False otherwise.
   */
  public function is_type($type) {
    if ($type === 'any' || $this->type === 'any') {
      return TRUE;
    }

    $a = is_array($type) ? $type : array($type);
    $b = is_array($this->type) ? $this->type : array($this->type);
    return (bool) array_intersect($a, $b);
  }

  /**
   * Return the argument.
   *
   * @return mixed
   *   The value of $argument.
   */
  public function get_argument() {
    return $this->argument;
  }

  /**
   * Return the value of argument (or arg) variable as it was passed in.
   *
   * @return mixed
   *   The original arg value.
   */
  public function get_original_argument() {
    if (!is_null($this->original_argument)) {
      return $this->original_argument;
    }
    return $this->argument;
  }

  /**
   * Return the keyword.
   *
   * @return mixed
   *   The value of $keyword.
   */
  public function get_keyword() {
    return $this->keyword;
  }

  /**
   * Return the identifier.
   *
   * @return mixed
   *   The value of $identifier.
   */
  public function get_identifier() {
    return $this->identifier;
  }

  /**
   * Return the title.
   *
   * @return mixed
   *   The value of $title.
   */
  public function get_title() {
    return $this->title;
  }

  /**
   * Return the page title.
   *
   * @return mixed
   *   The value of $page_title.
   */
  public function get_page_title() {
    return $this->page_title;
  }

}

/**
 * Used to create a method of comparing if a list of contexts
 * match a required context type.
 */
class og_context_required {
  /**
   * @var array
   *   Keyword strings associated with the context.
   */
  public $keywords;

  /**
   * If set, the title will be used in the selector to identify
   * the context. This is very useful when multiple contexts
   * are required to inform the user will be used for what.
   */
  public $title;

  /**
   * Test to see if this context is required.
   */
  public $required = TRUE;

  /**
   * If TRUE, skip the check in og_context_required::select()
   * for contexts whose names may have changed.
   */
  public $skip_name_check = FALSE;

  /**
   * The og_context_required constructor.
   *
   * Note: Constructor accepts a variable number of arguments, with optional
   * type-dependent args at the end of the list and one required argument,
   * the title. Note in particular that skip_name_check MUST be passed in as
   * a boolean (and not, for example, as an integer).
   *
   * @param string $title
   *   The title of the context for use in UI selectors when multiple contexts
   *   qualify.
   * @param string $keywords
   *   One or more keywords to use for matching which contexts are allowed.
   * @param array $restrictions
   *   Array of context restrictions.
   * @param bool $skip_name_check
   *   If True, skip the check in select() for contexts whose names may have
   *   changed.
   */
  public function __construct($title) {
    // If it was possible, using variadic syntax this should be:
    // __construct($title, string ...$keywords, array $restrictions = NULL, bool $skip = NULL)
    // but that form isn't allowed.
    $args = func_get_args();
    $this->title = array_shift($args);

    // If we have a boolean value at the end for $skip_name_check, store it.
    if (is_bool(end($args))) {
      $this->skip_name_check = array_pop($args);
    }

    // If we were given restrictions at the end, store them.
    if (count($args) > 1 && is_array(end($args))) {
      $this->restrictions = array_pop($args);
    }

    if (count($args) === 1) {
      $args = array_shift($args);
    }
    $this->keywords = $args;
  }

  /**
   * Filter the contexts to determine which apply in the current environment.
   *
   * A context passes the filter if:
   *  - the context matches 'type' of the required keywords (uses
   *    og_context::is_type(), so includes 'any' matches, etc).
   *  - AND if restrictions are present, there are some common elements between
   *    the requirement and the context.
   *
   * @param array $contexts
   *   An array of og_context objects (or something which will cast to an
   *   array of them). The contexts to apply the filter on.
   *
   * @return array
   *   An array of context objects, keyed with the same keys used for $contexts,
   *   which pass the filter.
   *
   * @see og_context::is_type()
   */
  public function filter($contexts) {
    $result = array();

    /**
     * See which of these contexts are valid.
     * @var og_context $context
     */
    foreach ((array) $contexts as $cid => $context) {
      if ($context->is_type($this->keywords)) {

        // Compare to see if our contexts were met.
        if (!empty($this->restrictions) && !empty($context->restrictions)) {

          foreach ($this->restrictions as $key => $values) {
            // If we have a restriction, the context must either not have that
            // restriction listed, which means we simply don't know what it is,
            // or there must be an intersection of the restricted values on
            // both sides.
            if (!is_array($values)) {
              $values = array($values);
            }

            if (!empty($context->restrictions[$key])
              && !array_intersect($values, $context->restrictions[$key])
            ) {
              // Break out to check next context; this one fails the filter.
              continue 2;
            }
          }
        }
        // This context passes the filter.
        $result[$cid] = $context;
      }
    }

    return $result;
  }

  /**
   * Select one context from the list of contexts, accounting for changed IDs.
   *
   * Fundamentally, this returns $contexts[$context] or FALSE if that does not
   * exist. Additional logic accounts for changes in context names and dealing
   * with a $contexts parameter that is not an array.
   *
   * If we had requested a $context but that $context doesn't exist in our
   * context list, there is a good chance that what happened is the context
   * IDs changed. Look for another context that satisfies our requirements,
   * unless $skip_name_check is set.
   *
   * @param og_context|array $contexts
   *   A context, or an array of og_context.
   * @param string $context
   *   A context ID.
   *
   * @return bool|og_context
   *   The matching og_context, or False if no such context was found.
   */
  public function select($contexts, $context) {
    // Easier to deal with a standalone object as a 1-element array of objects.
    if (!is_array($contexts)) {
      if (is_object($contexts) && $contexts instanceof og_context) {
        $contexts = array($contexts->id => $contexts);
      }
      else {
        $contexts = array($contexts);
      }
    }

    // If we had requested a $context but that $context doesn't exist in our
    // context list, there is a good chance that what happened is the context
    // IDs changed. Check for another context that satisfies our requirements.
    if (!$this->skip_name_check
      && !empty($context) && !isset($contexts[$context])
    ) {
      $choices = $this->filter($contexts);

      // If we got a hit, take the first one that matches.
      if ($choices) {
        $keys = array_keys($choices);
        $context = reset($keys);
      }
    }

    if (empty($context) || empty($contexts[$context])) {
      return FALSE;
    }
    return $contexts[$context];
  }

}

/**
 * Used to compare to see if a list of contexts match an optional context. This
 * can produce empty contexts to use as placeholders.
 */
class og_context_optional extends og_context_required {

  /**
   * {@inheritdoc}
   */
  public $required = FALSE;

  /**
   * Add the 'empty' context to the existing set.
   *
   * @param array &$contexts
   *   An array of og_context objects.
   */
  public function add_empty(&$contexts) {
    $context = new og_context('any');
    $context->title = t('No context');
    $context->identifier = t('No context');
    $contexts['empty'] = $context;
  }

  /**
   * Filter the contexts to determine which apply in the current environment.
   *
   * As for og_context_required, but we add the empty context to those
   * passed in so the check is optional (i.e. if nothing else matches, the
   * empty context will, and so there will always be at least one matched).
   *
   * @param array $contexts
   *   An array of og_context objects (or something which will cast to an
   *   array of them). The contexts to apply the filter on.
   *
   * @return array
   *   An array of context objects, keyed with the same keys used for $contexts,
   *   which pass the filter.
   *
   * @see og_context::is_type()
   */
  public function filter($contexts) {
    /**
     * @todo We are assuming here that $contexts is actually an array, whereas
     * og_context_required::filter only requires $contexts is convertible
     * to an array.
     */
    $this->add_empty($contexts);
    return parent::filter($contexts);
  }

  /**
   * Select and return one context from the list of applicable contexts.
   *
   * Fundamentally, this returns $contexts[$context] or the empty context if
   * that does not exist.
   *
   * @param array $contexts
   *   The applicable contexts to check.
   * @param string $context
   *   The context id to check for.
   *
   * @return bool|og_context
   *   The matching og_context, or False if no such context was found.
   *
   * @see og_context_required::select()
   */
  public function select($contexts, $context) {
    /**
     * @todo We are assuming here that $contexts is actually an array, whereas
     * og_context_required::select permits og_context objects as well.
     */
    $this->add_empty($contexts);
    if (empty($context)) {
      return $contexts['empty'];
    }

    $result = parent::select($contexts, $context);

    // Don't flip out if it can't find the context; this is optional, put
    // in an empty.
    if ($result === FALSE) {
      $result = $contexts['empty'];
    }
    return $result;
  }
}
