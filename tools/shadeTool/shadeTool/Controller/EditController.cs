﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using shadeTool.Models;

namespace shadeTool.Controller
{
    public class EditController
    {
        public delegate void BrushHandler(SceneBrush brush);
        public event BrushHandler ActiveBrushChanged;

        public int DrawMode = 0;

        public const int DRAWMODE_FLOOR = 0;
        public const int DRAWMODE_WALL = 1;

        private SceneBrush _ActiveBrush;
        public SceneBrush ActiveBrush
        {
            get
            {
                return _ActiveBrush;
            }
            set
            {
                _ActiveBrush = value;
                if (ActiveBrushChanged != null)
                {
                    ActiveBrushChanged(_ActiveBrush);
                }
            }
        }
        public string ActiveStyleKey { get; set; }

        public int z_layer { get; set; }

        public EditController()
        {
            z_layer = 0;
        }
    }
}
