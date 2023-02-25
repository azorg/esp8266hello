/*
 * File: "cli.c"
 */
//-----------------------------------------------------------------------------
#include <stdint.h>
#include <stdlib.h>
#include <Arduino.h>
#include "config.h"
#include "print.h"
#include "cli.h"
#include "mrl.h"
#include "global.h"
#include "limit.h"
//-----------------------------------------------------------------------------
#define CLI_COMPL_NUM 50 // max variant of MicroRL complettion
#define CLI_MAX_CMD   10 // maximum number of tokens
//-----------------------------------------------------------------------------
#define CLI_KEYCODE_CTRL_C  3 // Ctrl+C key code
#define CLI_KEYCODE_CTRL_D  4 // Ctrl+D key code
#define CLI_KEYCODE_CTRL_S 19 // Ctrl+S key code
#define CLI_KEYCODE_CTRL_T 20 // Ctrl+T key code
#define CLI_KEYCODE_CTRL_Z 26 // Ctrl+Z key code
#define CLI_KEYCODE_CTRL_V 22 // Ctrl+V key code
#define CLI_KEYCODE_CTRL_O 15 // Ctrl+O key code
#define CLI_KEYCODE_CTRL_Q 17 // Ctrl+Q key code
#define CLI_KEYCODE_CTRL_W 23 // Ctrl+W key code
#define CLI_KEYCODE_CTRL_X 24 // Ctrl+X key code
#define CLI_KEYCODE_CTRL_Y 25 // Ctrl+Y key code
//-----------------------------------------------------------------------------
#ifdef MRL_USE_COMPLETE
// array for MicroRL comletion
static const char *cli_compl_world[CLI_COMPL_NUM + 1];
#endif // MRL_USE_COMPLETE
//-----------------------------------------------------------------------------
#ifndef MRL_PRINT_ESC_OFF
const char *cli_prompt = "\033[32m$\033[0m "; // color prompt
#else
const char *cli_prompt = "$ "; // no color
#endif
int cli_prompt_len = 2;
//-----------------------------------------------------------------------------
// Arduino wrapper for MicroRL
static void cli_print(const char *str)
{
  Serial.print(str);
}
//-----------------------------------------------------------------------------
// получить код нажатой клавиши
int cli_inkey()
{ // return -1 if no data is available
  return Serial.read();
}
//-----------------------------------------------------------------------------
// execute callback for microrl library
static void cli_execute_cb(int argc, char * const argv[])
{
  int i, parent_id = -1, arg_shift;
  const cli_cmd_t *found = (cli_cmd_t*) NULL;

  for (i = 0; i < argc; i++)
  {
    const cli_cmd_t *cmd = cli_tree;
    while (cmd->name != NULL)
    {
      if (cmd->parent_id == parent_id && strcmp(cmd->name, argv[i]) == 0)
      { // command/option found
        found     = cmd;
        parent_id = cmd->id;
        arg_shift = i + 1;
        break;
      }
      cmd++;
    } // while
  } // for

  if (found != (cli_cmd_t*) NULL) // command found
    found->fn(argc - arg_shift, argv + arg_shift, found);
  else
  {
    print_str("command ");
    print_str(argv[0]);
    print_str(" not found\r\n");
  }
}
//-----------------------------------------------------------------------------
#ifdef MRL_USE_COMPLETE
// completion callback for microrl library
static const char** cli_complete_cb(int argc, char * const argv[])
{
  int i, parent_id = -1, count = 0;

  for (i = 0; i < argc ; i++)
  {
    const cli_cmd_t *cmd = cli_tree;
    while (cmd->name != NULL)
    {
      if (cmd->parent_id == parent_id &&
          strstr(cmd->name, argv[i]) == cmd->name &&
          (parent_id != -1 || i == 0))
      { // substring found => add it to completion set

        if (i == argc - 1 && count < CLI_COMPL_NUM)
          cli_compl_world[count++] = cmd->name;

        if (strcmp(cmd->name, argv[i]) == 0)
        { // command/option full found
          parent_id = cmd->id;
          break;
        }
      }
      cmd++;
    } // while
  } // for

  cli_compl_world[count] = NULL;
  return cli_compl_world;
}
#endif // MRL_USE_COMPLETE
//-----------------------------------------------------------------------------
#ifdef CLI_HELP
// find command record by ID
static const cli_cmd_t *cli_cmd_by_id(int id)
{
  const cli_cmd_t *o;
  for (o = cli_tree; o->name != NULL; o++)
    if (o->id == id)
      break;
  return o;
}
#endif // CLI_HELP
//-----------------------------------------------------------------------------
// show help based on command/option tree
void cli_help(int argc, char* const argv[], const cli_cmd_t *cmd)
{ // help
#ifdef CLI_HELP
  if (cmd->id == 0)
  { // root (full) help
    const cli_cmd_t *o;

    print_str(ABOUT_STRING "\r\n"
              "Use TAB key for completion\r\n"
              "Command:\r\n");

    for (o = cli_tree; o->name != NULL; o++)
    {
      int i, parents_cnt = 0;
      const cli_cmd_t *parents[CLI_MAX_CMD];
      const cli_cmd_t *p = o;

      // find all parents
      for (i = 0; i < CLI_MAX_CMD; i++)
        if (p->parent_id >= 0)
          p = parents[parents_cnt++] = cli_cmd_by_id((int) p->parent_id);
        else
          break;

      // print all parents
      print_chr(' ');
      while (--parents_cnt >= 0)
      {
        print_str(parents[parents_cnt]->name);
        print_chr(' ');
      }

      // print command and help
#ifndef MRL_PRINT_ESC_OFF
      print_str(o->name);
      print_str("\033[33m");
      print_str(o->args);
      print_str("\033[0m - \033[32m");
      print_str(o->help);
      print_str("\033[0m\r\n");
#else
      print_str(o->name);
      print_str(o->args);
      print_str(" - ");
      print_str(o->help);
      print_str("\r\n");
#endif
    } // for
  }
  else
  { // command (partial) help
    const cli_cmd_t *o;

    // print command help header
    print_str(cmd->help);
    print_str(":\r\n");

    // find all parent/children
    for (o = cli_tree; o->name != NULL; o++)
    {
      int i, flg = 0, parents_cnt = 0;
      const cli_cmd_t *parents[CLI_MAX_CMD];
      const cli_cmd_t *p = o;

      // find all parents
      for (i = 0; i < CLI_MAX_CMD; i++)
      {
        if (p->parent_id >= 0)
        {
          if (p->parent_id == cmd->id)
            flg = 1;
          p = parents[parents_cnt++] = cli_cmd_by_id((int) p->parent_id);
        }
        else
          break;
      }

      if (flg)
      {
        // print all parents
        print_chr(' ');
        while (--parents_cnt >= 0)
        {
          print_str(parents[parents_cnt]->name);
          print_chr(' ');
        }

        // print command and help
#ifndef MRL_PRINT_ESC_OFF
        print_str(o->name);
        print_str("\033[33m");
        print_str(o->args);
        print_str("\033[0m - \033[32m");
        print_str(o->help);
        print_str("\033[0m\r\n");
#else
        print_str(o->name);
        print_str(o->args);
        print_str(" - ");
        print_str(o->help);
        print_str("\r\n");
#endif
      }
    } // for
  }
#endif // CLI_HELP
}
//=============================================================================
void cli_version(int argc, char* const argv[], const cli_cmd_t *cmd)
{ // version
  print_str("hw_revision="  VERSION_HARDWARE_STR "\r\n"
            "soft_version=" VERSION_SOFTWARE_STR "\r\n");
}
//-----------------------------------------------------------------------------
void cli_clear(int argc, char* const argv[], const cli_cmd_t *cmd)
{ // clear
#ifndef MRL_PRINT_ESC_OFF
  print_str("\033[2J"  // ESC seq for clear entire screen
            "\033[H"); // ESC seq for move cursor at left-top corner
#else
  print_str("\r\n\r\n");
#endif
}
//-----------------------------------------------------------------------------
void cli_verbose(int argc, char* const argv[], const cli_cmd_t *cmd)
{ // verbose [n]
  if (argc)
  {
    Verbose = mrl_str2int(argv[0], Verbose, 0);
    Verbose = LIMIT(Verbose, 0, 3);
    print_str("set ");
  }
  print_ival("verbose=", Verbose);
}
//-----------------------------------------------------------------------------
void cli_led(int argc, char* const argv[], const cli_cmd_t *cmd)
{ // led {0|1}
  if (argc == 1)
  {
    int on = !!mrl_str2int(argv[0], 0, 10);
    Led.blink(0);
    Led.set(on);
    if (Verbose)
      print_ival("LED=", on);
  }
}
//-----------------------------------------------------------------------------
void cli_led_blink(int argc, char* const argv[], const cli_cmd_t *cmd)
{ // led blink [N]
  int i, n = 1;
  if (argc > 0) n = mrl_str2int(argv[0], 0, 10);
  Led.blink(n);

  if (n && Verbose)
  {
    print_str("blink LED ");
    print_int(n);
    print_str(" times\r\n");
  }
}
//-----------------------------------------------------------------------------
void cli_sys_info(int argc, char* const argv[], const cli_cmd_t *cmd)
{ // sys info
  //...
}
//-----------------------------------------------------------------------------
void cli_sys_time(int argc, char* const argv[], const cli_cmd_t *cmd)
{ // sys time
  print_uval("millis() = ", millis());
  print_uval("micros() = ", micros());
  print_eol();
}
//-----------------------------------------------------------------------------
void cli_sys_reset(int argc, char* const argv[], const cli_cmd_t *cmd)
{ // sys reset
  print_str("\rreset\r\n");
  print_flush();
  //...
}
//=============================================================================
#ifdef MRL_USE_CTRL_C
// Ctrl+C callback
static void cli_sigint_cb()
{
  mrl_prompt(&Mrl);
}
#endif // MRL_USE_CTRL_C
//-----------------------------------------------------------------------------
void cli_init()
{
  // init command parser
  mrl_init(&Mrl, cli_print);
  mrl_set_execute_cb(&Mrl, cli_execute_cb);
  mrl_set_prompt(&Mrl, cli_prompt, cli_prompt_len);

#if defined(MRL_USE_COMPLETE)
  mrl_set_complete_cb(&Mrl, cli_complete_cb); // set callback for completion
#endif // MRL_USE_COMPLETE

#ifdef MRL_USE_CTRL_C
  mrl_set_sigint_cb(&Mrl, cli_sigint_cb);
#endif // MRL_USE_CTRL_C

#if 1
  // clear input buffer
  while (cli_inkey() != -1);
#endif

  // show prompt
  //mrl_prompt(&Mrl);
}
//-----------------------------------------------------------------------------
void cli_loop()
{
  int key;

  while ((key = cli_inkey()) != -1)
    if ((key = mrl_insert_char(&Mrl, (char) key)) != 0)
      break;

  // check special keys
  if (key == CLI_KEYCODE_CTRL_D) // Ctrl+D pressed
  {
    print_str("\r\n^D\r\n");
    //...
    mrl_refresh(&Mrl);
  }
  else if (key == CLI_KEYCODE_CTRL_T) // Ctrl+T pressed
  {
    print_str("\r\n^T\r\n");
    //...
    mrl_refresh(&Mrl);
  }
  else if (key == CLI_KEYCODE_CTRL_S) // Ctrl+S pressed
  {
    print_str("\r\n^S\r\n");
    //...
    mrl_refresh(&Mrl);
  }
#ifndef MRL_USE_CTRL_C
  else if (key == CLI_KEYCODE_CTRL_C) // Ctrl+C pressed
  {
    print_str("\r\n^C\r\n");
    //...
    mrl_prompt(&Mrl);
  }
#endif // !MRL_USE_CTRL_C
  else if (key == CLI_KEYCODE_CTRL_Z) // Ctrl+Z pressed
  {
    print_str("\r\n^Z\r\n");
    //...
    mrl_refresh(&Mrl);
  }
  else if (key == CLI_KEYCODE_CTRL_V) // Ctrl+V pressed
  {
    print_str("\r\n^V\r\n");
    //...
    mrl_refresh(&Mrl);
  }
  else if (key == CLI_KEYCODE_CTRL_Q) // Ctrl+Q pressed
  {
    print_str("\r\n^Q\r\n");
    //...
    mrl_refresh(&Mrl);
  }
  else if (key == CLI_KEYCODE_CTRL_W) // Ctrl+W pressed
  {
    print_str("\r\n^W\r\n");
    //...
    mrl_refresh(&Mrl);
  }
  else if (key == CLI_KEYCODE_CTRL_Y) // Ctrl+Y pressed
  {
    print_str("\r\n^Y\r\n");
    //...
    mrl_refresh(&Mrl);
  }
#ifdef CLI_HELP
  else if (key == CLI_KEYCODE_CTRL_X) // Ctrl+X pressed
  {
    print_str("\r\n^X\r\n");
    cli_help(0, NULL, &cli_tree[0]);
    mrl_refresh(&Mrl);
  }
#endif // CLI_HELP
}
//-----------------------------------------------------------------------------
// all commands and options tree
#include "cli_tree.h"
//-----------------------------------------------------------------------------

/*** end of "cli.c" file ***/


